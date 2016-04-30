// FindPathLib.cpp : Defines the exported functions for the DLL application.
//

#include "FindPathLib.h"


// This is an example of an exported variable
FINDPATHLIB_API int nFindPathLib=0;

// This is an example of an exported function.
FINDPATHLIB_API int fnFindPathLib(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see FindPathLib.h for the class definition
CFindPathLib::CFindPathLib()
{
    return;
}
