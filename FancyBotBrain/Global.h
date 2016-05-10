#pragma once

#include <hadesmem/process.hpp>

static hadesmem::Process& GetThisProcess()
{
	static hadesmem::Process process{ GetCurrentProcessId() };
	return process;
}
