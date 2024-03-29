#pragma once

#ifdef GUINSOODB_BUILD_LOADABLE_EXTENSION
#ifdef _WIN32
#include <windows.h>
#include <delayimp.h>

extern "C" {
/*
This is interesting: Windows would normally require a guinsoodb.dll being on the DLL search path when we load an extension
using LoadLibrary(). However, there is likely no such dll, because GuinsooDB was statically linked, or is running as part
of an R or Python module with a completely different name (that we don't know) or something of the sorts. Amazingly,
Windows supports lazy-loading DLLs by linking them with /DELAYLOAD. Then a callback will be triggerd whenever we access
symbols in the extension. Since GuinsooDB is already running in the host process (hopefully), we can use
GetModuleHandle(NULL) to return the current process so the symbols are looked for there. See here for another
explanation of this crazy process:

* https://docs.microsoft.com/en-us/cpp/build/reference/linker-support-for-delay-loaded-dlls?view=msvc-160
* https://docs.microsoft.com/en-us/cpp/build/reference/understanding-the-helper-function?view=msvc-160
*/
FARPROC WINAPI guinsoodb_dllimport_delay_hook(unsigned dliNotify, PDelayLoadInfo pdli) {
	switch (dliNotify) {
	case dliNotePreLoadLibrary:
		if (strcmp(pdli->szDll, "guinsoodb.dll") != 0) {
			return NULL;
		}
		return (FARPROC)GetModuleHandle(NULL);
	default:
		return NULL;
	}

	return NULL;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = guinsoodb_dllimport_delay_hook;
ExternC const PfnDliHook __pfnDliFailureHook2 = guinsoodb_dllimport_delay_hook;
}
#endif
#endif