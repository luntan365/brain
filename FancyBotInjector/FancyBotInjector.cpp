// FancyBotInjector.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <hadesmem/debug_privilege.hpp>
#include <hadesmem/injector.hpp>
#include <hadesmem/process_helpers.hpp>

int InjectIntoRunningProcess()
{
	auto const proc_name = L"WoW.exe";
	const std::wstring module_path = L"C:\\cygwin64\\home\\kphelps\\dev\\build\\Debug\\FancyBotBrain.dll";

	std::wcout << "Finding exe...\n";

	try {
		auto process = hadesmem::GetProcessByName(proc_name);

		std::wcout << "Injecting Brain into exe...\n";

		HMODULE module = hadesmem::InjectDll(process, module_path, hadesmem::InjectFlags::kNone);

		std::wcout << "\nSuccessfully injected module at base "
			"address " << hadesmem::detail::PtrToHexString(module) << ".\n";

		auto const export_name = "BrainMain";
		hadesmem::CallResult<DWORD_PTR> const export_ret =
			hadesmem::CallExport(process, module, export_name);

		std::wcout << "\nSuccessfully called module export.\n";
		std::wcout << "Return: " << export_ret.GetReturnValue() << ".\n";
		std::wcout << "LastError: " << export_ret.GetLastError() << ".\n";
		return 0;
	} catch (const hadesmem::Error err) {
		std::wcerr << "\n\n--- FAILED ---\n\n" << err.what();
		std::cerr << boost::current_exception_diagnostic_information() << '\n';
		return 1;
	}
}

int LaunchWithHacks()
{
	const std::wstring exe_path = L"C:\\Users\\Kyle\\Downloads\\World of Warcraft 1.12\\WoW.exe";
	const std::wstring work_dir = L"C:\\Users\\Kyle\\Downloads\\World of Warcraft 1.12";
	std::vector<std::wstring> exe_args;
	const std::wstring module_path = L"C:\\Users\\Kyle\\Documents\\Visual Studio 2015\\Projects\\PathFinder\\Debug\\FancyBotBrain.dll";
	const std::string export_name = "BrainMain";
	auto flags = hadesmem::InjectFlags::kNone;

	hadesmem::CreateAndInjectData const inject_data =
		hadesmem::CreateAndInject(
			exe_path,
			work_dir,
			std::begin(exe_args),
			std::end(exe_args),
			module_path,
			export_name,
			flags,
			0
		);

	std::wcout << "\nSuccessfully created target.\n";
	std::wcout << "Process ID: " << inject_data.GetProcess() << ".\n";
	std::wcout << "Module Base: "
	<< hadesmem::detail::PtrToHexString(inject_data.GetModule())
	<< ".\n";
	std::wcout << "Export Return: " << inject_data.GetExportRet() << ".\n";
	std::wcout << "Export LastError: " << inject_data.GetExportLastError()
	<< ".\n";
	return 0;
}

int main()
{
	try
	{
		hadesmem::GetSeDebugPrivilege();

		std::wcout << "\nAcquired SeDebugPrivilege.\n";
	}
	catch (std::exception const& /*e*/)
	{
		std::wcout << "\nFailed to acquire SeDebugPrivilege.\n";
	}
	//TestCodeGen();
	return InjectIntoRunningProcess();
	//return LaunchWithHacks();
}

