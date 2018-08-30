// entry point for the console application.

#ifdef  _WIN32
    #include "stdafx.h"
#endif
#include "VoxelizationTestScene.h"

int main()
{
	e186::Engine::StartWindowedWithRootScene(1600, 900, []()
	{
		return std::make_unique<e186::VoxelizationTestScene>();
	});
}

