// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <windows.h>
#include <filesystem>
#include <ShlObj.h>
#include <iostream>
#include <TlHelp32.h>

VOID startup(const char* lpApplicationName)
{
	// additional information
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcessA(lpApplicationName,   // the path
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

INT __stdcall WinMain(HINSTANCE, HINSTANCE,
	PSTR, INT)
{
	//get path of the folder where GoXLR stores config data
	CHAR path[MAX_PATH];
	if (!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, path)))
		return 1;
	std::string path_s = path;

	//close the GoXLR app and wait for it to shut down
	system("taskkill /IM \"GOXLR App.exe\" /F");
	Sleep(100);

	//replace the currently loaded file with a saved profile
	std::filesystem::remove(path_s + "\\GoXLR\\Profiles\\Custom.goxlr"); //Custom.goxlr is out main profile
	std::filesystem::copy_file(path_s + "\\GoXLR\\Profiles\\Saved\\Custom.goxlr", path_s + "\\GoXLR\\Profiles\\Custom.goxlr"); //Saved-Custom.goxlr is the saved profile
	Sleep(50);

	//relaunch the app so it loads up our new profile
	startup("C:\\Program Files (x86)\\TC-Helicon\\GOXLR\\GoXLR App.exe");
	
	return 0;
}
