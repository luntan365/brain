// FancyBotBrain.cpp : Defines the exported functions for the DLL application.
//


#include "stdafx.h"
#include "http_server.h"

#include <Windows.h>
#include <shellapi.h>
#include <d3d9.h>
#include "FancyBotBrain.h"
#include "asmjit/asmjit.h"
#include "hadesmem/alloc.hpp"
#include "hadesmem/patcher.hpp"
#include "hadesmem/process.hpp"

#include "EndSceneManager.h"
#include "Global.h"
#include "GameState.h"
#include "GrindBot.h"
#include "HttpApi.h"
#include "WoWPlayer.h"
#include "WowOffsets.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

DWORD_PTR GetEndSceneAddress(const hadesmem::Process& process)
{
	uintptr_t callerOffset = 0x5A17B6;
	auto endSceneAddrAllocator = hadesmem::Allocator(process, 4);
	auto pEndSceneAddr = endSceneAddrAllocator.GetBase();
	auto isReadyAllocator = hadesmem::Allocator(process, 4);
	auto pIsReady = isReadyAllocator.GetBase();

	hadesmem::Write<uint32_t>(process, pIsReady, 0);

	asmjit::JitRuntime runtime;
	asmjit::X86Assembler assembler{ &runtime };
	asmjit::StringLogger logger;
	assembler.setLogger(&logger);
	assembler.pushf();
	assembler.pusha();
	assembler.mov(
		asmjit::x86::eax,
		asmjit::x86::ptr(asmjit::x86::ecx, 0xA8)
	);
	assembler.mov(
		asmjit::x86::ptr_abs((uintptr_t)pEndSceneAddr),
		asmjit::x86::eax
	);
	assembler.mov(asmjit::x86::eax, 1);
	assembler.mov(
		asmjit::x86::ptr_abs((uintptr_t)pIsReady),
		asmjit::x86::eax
	);
	assembler.popa();
	assembler.popf();
	assembler.call(asmjit::x86::ptr(asmjit::x86::ecx, 0xA8));
	assembler.jmp(callerOffset + 0x6);

	HADESMEM_DETAIL_TRACE_A("Allocating memory for remote stub.");

	DWORD_PTR const stub_size = assembler.getCodeSize();

	hadesmem::Allocator stub_mem_remote{ process, stub_size };

	std::vector<BYTE> code_real(stub_size);
	assembler.relocCode(
		code_real.data(),
		reinterpret_cast<DWORD_PTR>(stub_mem_remote.GetBase())
	);
	WriteVector(process, stub_mem_remote.GetBase(), code_real);
	FlushInstructionCache(process, stub_mem_remote.GetBase(), stub_size);

	HADESMEM_DETAIL_TRACE_A(logger.getString());
	HADESMEM_DETAIL_TRACE_A("Generating detour code");

	assembler.reset();
	logger.clearString();
	assembler.jmp((uintptr_t)stub_mem_remote.GetBase());
	assembler.nop();
	std::vector<BYTE> detour_jmp(6);
	assembler.relocCode(
		detour_jmp.data(),
		callerOffset
	);
	auto callerAddr = (void*)callerOffset;
	WriteVector(process, callerAddr, detour_jmp);
	FlushInstructionCache(process, callerAddr, detour_jmp.size());

	HADESMEM_DETAIL_TRACE_A(logger.getString());

	while (hadesmem::Read<uint32_t>(process, pIsReady) == 0)
	{
	}

	std::vector<BYTE> code_old{ 0xFF, 0x91, 0xA8, 0x00, 0x00, 0x00 };
	WriteVector(process, callerAddr, code_old);
	FlushInstructionCache(process, callerAddr, code_old.size());

	auto addr = hadesmem::Read<DWORD_PTR>(process, pEndSceneAddr);

	endSceneAddrAllocator.Free();
	isReadyAllocator.Free();
	stub_mem_remote.Free();

	return addr;
}

typedef HRESULT(WINAPI* EndScene_Fn)(IDirect3DDevice9* device);
typedef hadesmem::PatchDetour<EndScene_Fn> EndSceneDetourT;

std::unique_ptr<EndSceneDetourT>&
GetEndSceneDetour() noexcept
{
	static std::unique_ptr<EndSceneDetourT> detour;
	return detour;
}

static int frameIters = 0;

void UpdateGameState()
{
    auto& gs = GameState::Instance();
    auto lock = gs.GetLock();
    if (lock.try_lock())
    {
        gs.Update();
    }
}

extern "C" HRESULT WINAPI EndSceneHook(
	hadesmem::PatchDetourBase* detour,
	IDirect3DDevice9* device)
{
	auto& manager = EndSceneManager::Instance();

	// TODO: Will probably want to give this a certain amount of time to execute
	// in each frame rather than executing everything at once
	while (!manager.Empty())
	{
		manager.EvaluateNextFunction();
	}

    if (frameIters % 100 == 0)
    {
        manager.Execute(UpdateGameState);
    }
    frameIters++;

	auto const end_scene = detour->GetTrampolineT<EndScene_Fn>();
	return end_scene(device);
}

void HookEndScene(const hadesmem::Process& process, DWORD_PTR addr)
{
	auto& pDetour = GetEndSceneDetour();
	pDetour.reset(new EndSceneDetourT(process, (EndScene_Fn)addr, &EndSceneHook));
	pDetour->Apply();
}

void UnhookEndScene()
{
	auto& pDetour = GetEndSceneDetour();
	pDetour->Remove();
}

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

DWORD __stdcall StartHTTPServer(LPVOID args)
{
	HADESMEM_DETAIL_TRACE_A("Starting HTTP Server...");
    HttpApi server(8080, 4);
    server.Start();
	HADESMEM_DETAIL_TRACE_A("HTTP Server Closed");
    return 0;
}

DWORD __stdcall StartBot(LPVOID args)
{
    HADESMEM_DETAIL_TRACE_A("Starting bot...");
    GrindBot bot;
    bot.OnStart();
    while (true)
    {
        std::this_thread::sleep_for(100ms);
        auto& gs = GameState::Instance();
        auto lock = gs.GetLock();
        if (gs.ObjectManager().GetPlayer().GetAddress() == nullptr)
        {
            // Object manager hasn't been tick'd yet
            continue;
        }
        lock.lock();
        bot.Tick(gs);
    }
}

FANCYBOTBRAIN_API DWORD_PTR BrainMain(void)
{
	auto& process = GetThisProcess();

	HADESMEM_DETAIL_TRACE_A("Retrieving EndScene Address");
	auto addr = GetEndSceneAddress(process);
	HADESMEM_DETAIL_TRACE_FORMAT_A("Retrieving EndScene Address: %d", addr);

	HADESMEM_DETAIL_TRACE_A("Setting up EndScene hook");
	HookEndScene(process, addr);
	HADESMEM_DETAIL_TRACE_A("EndScene hooked, lets do this");

	DWORD httpThreadId;
	auto httpThreadHandle = CreateThread(nullptr, 0, &StartHTTPServer, nullptr, 0, &httpThreadId);

    DWORD botThreadId;
    auto botThreadHandle = CreateThread(nullptr, 0, &StartBot, nullptr, 0, &botThreadId);

    return 0;
}

FANCYBOTBRAIN_API DWORD_PTR BrainUnload(void)
{
	UnhookEndScene();
	return 0;
}
