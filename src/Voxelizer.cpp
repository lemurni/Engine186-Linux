#include "Voxelizer.h"

namespace e186
{

    Voxelizer::Voxelizer() :
	    m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Voxelizer"))
	{

		/*
		m_mesh_to_voxel_rasterization_shader
			.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Geometry | ShaderType::Fragment)
			.AddVertexShaderSourceFromFile("assets/shaders/voxelize.vert")
			.AddGeometryShaderSourceFromFile("assets/shaders/voxelize.geom")
			.AddFragmentShaderSourceFromFile("assets/shaders/voxelize.frag")
			.Build();
		*/

		TwDefine("'Voxelizer' color='26 27 61' text=light position='50 50' ");

		TwEnumVal voxelStorageModeEV[] = {
		    { static_cast<int>(VoxelStorageMode::RegularGridTex3D), "RegularGridTex3D" },
		    { static_cast<int>(VoxelStorageMode::OctreeHierarchy), "OctreeHierarchy" }
		};
		TwType voxelStorageModeTWType = TwDefineEnum("Voxel Storage Mode", voxelStorageModeEV, 2);
		TwAddVarRW(m_tweak_bar, "Voxel Storage Mode", voxelStorageModeTWType, &m_voxel_storage_mode, "");
	}

	Voxelizer::~Voxelizer()
	{
	}

	void Voxelizer::Voxelize(std::unique_ptr<Model>& sourceMeshModel)
	{
		// TODO IMPLEMENT
		std::cout << "Voxelizer::Voxelize(const Model &sourceMeshModel) not yet implemented." << std::endl;

	}

	void Voxelizer::RenderVoxelGrid()
	{
		// TODO IMPLEMENT
		std::cout << "Voxelizer::RenderVoxelGrid() not yet implemented." << std::endl;
	}

}
