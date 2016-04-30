// FindPathLibHarness.cpp : Defines the entry point for the console application.
//

#include "MoveMapManager.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP


int main()
{
	MoveMapManager manager;
	manager.Initialize("C:\\mmaps");
	return 0;
}

