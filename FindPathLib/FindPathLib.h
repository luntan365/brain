// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FINDPATHLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FINDPATHLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FINDPATHLIB_EXPORTS
#define FINDPATHLIB_API __declspec(dllexport)
#else
#define FINDPATHLIB_API __declspec(dllimport)
#endif

// This class is exported from the FindPathLib.dll
class FINDPATHLIB_API CFindPathLib {
public:
	CFindPathLib(void);
	// TODO: add your methods here.
};

extern FINDPATHLIB_API int nFindPathLib;

FINDPATHLIB_API int fnFindPathLib(void);
