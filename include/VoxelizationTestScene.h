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
		VoxelizationTestScene(std::string model_to_load_path, glm::mat4 transformation_matrix, unsigned int model_loader_flags = MOLF_default);
		virtual ~VoxelizationTestScene();
		void Terminate() override;
		void Run() override;

	private:

		bool m_termination_requested;

		Voxelizer m_voxelizer;

		std::unique_ptr<Model> m_model;
	};

}
