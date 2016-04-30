// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FANCYBOTBRAIN_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FANCYBOTBRAIN_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FANCYBOTBRAIN_EXPORTS
#define FANCYBOTBRAIN_API __declspec(dllexport)
#else
#define FANCYBOTBRAIN_API __declspec(dllimport)
#endif

extern "C" FANCYBOTBRAIN_API DWORD_PTR BrainMain(void);
extern "C" FANCYBOTBRAIN_API DWORD_PTR BrainUnload(void);
