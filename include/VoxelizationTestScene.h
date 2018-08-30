#include "MaxFpsTimer.h"
#include "QuakeCamera.h"
#include <glad/glad.h>
#include "Stopwatch.h"

#include "Voxelizer.h"

#pragma once
namespace e186
{

    class VoxelizationTestScene : public IScene
	{
	public:
		VoxelizationTestScene();
		virtual ~VoxelizationTestScene();
		void Terminate() override;
		void Run() override;

	private:

		bool m_termination_requested;

		std::unique_ptr<Voxelizer> m_voxelizer;
		std::unique_ptr<Model> m_voxelize_model;

		std::unique_ptr<Model> m_scene_model;
	};

}
