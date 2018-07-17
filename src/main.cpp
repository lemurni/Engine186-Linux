// entry point for the console application.

#ifdef  _WIN32
    #include "stdafx.h"
#endif
#include "VoxelizationTestScene.h"

int main()
{
	e186::Engine::StartWindowedWithRootScene(1600, 900, []()
	{
		// path to mesh for testing voxelization
		std::string modelPath = "assets/models/sponza/sponza_structure.obj";

		return std::make_unique<e186::VoxelizationTestScene>(modelPath, glm::scale(glm::vec3(.01f, .01f, .01f)));
	});
}

