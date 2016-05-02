// FindPathLibHarness.cpp : Defines the entry point for the console application.
//

#include "easylogging++.h"
#include "MoveMapManager.h"
#include "PathFinder.h"


int main()
{
	MoveMapManager manager;
	manager.Initialize("C:\\mmaps");
    PathFinder finder(&manager, 0);
    finder.Calculate(
        Vector3(
            -8936.78125,
            -148.124847,
            82.7936172
        ),
        Vector3(
            -8919.37891,
            -230.486176,
            81.3935623
        )
    );

    for (const auto& point : finder.GetPath())
    {
        auto x = point.x;
        auto y = point.y;
        auto z = point.z;
        LOG(INFO) << "(" << x << ", " << y << ", " << z << ")";
    }

	return 0;
}

