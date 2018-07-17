#pragma once

// note: several header are included by the precompiled prefix header include/e186.h
#include "Tex2D.h"
#include "Tex3D.h"
#include "FrameBufferObject.h"

namespace e186
{

	class Voxelizer
	{

	public:

		enum struct VoxelStorageMode
	    {
		    RegularGridTex3D,
		    OctreeHierarchy
	    };
		

	private:
	
		AntTweakBarHandle m_tweak_bar;

		VoxelStorageMode m_voxel_storage_mode;

		// the mesh model to be voxelized
		std::unique_ptr<Model> m_source_mesh_model;

		// shader for the gpu rasterizer based voxelization
		Shader m_mesh_to_voxel_rasterization_shader;

		// OpenGL 3D Texture to store voxels in regular grid
		// if RegularGridTex3D enabled, stores result of latest call to Voxelizer::Voxelize()
		Tex3D m_voxels_tex3D;

		// Octree data structure for more efficient voxel storage
		// if OctreeHierarchy enabled, stores result of latest call to Voxelizer::Voxelize()
		//VoxelOctree m_voxels_octree;


	public:
	
		Voxelizer();
		~Voxelizer();

		// voxelize the given mesh and store the result in a data structure
		// the resulting data structure is determined by m_voxel_storage_mode
		void Voxelize(std::unique_ptr<Model> &sourceMeshModel);

		// visualize the voxels
		void RenderVoxelGrid();
	};

}
