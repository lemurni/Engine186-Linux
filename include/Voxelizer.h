#pragma once

// note: several header are included by the precompiled prefix header include/e186.h
#include "Tex2D.h"
#include "Tex3D.h"
#include "FrameBufferObject.h"

#include "e186.h"

namespace e186
{

	class Voxelizer
	{

	public:

		enum struct VoxelStorageMode
	    {
			Tex3D = 0,
			OctreeHierarchy = 1
	    };
		

	private:
	
		AntTweakBarHandle m_tweak_bar;

		// VOXELIZATION AND VOXEL DATA

		VoxelStorageMode m_voxel_storage_mode;

		// the mesh model to be voxelized
		std::unique_ptr<Model> m_source_mesh_model;

		// shader for the gpu rasterizer based voxelization
		Shader m_mesh_to_voxel_rasterization_shader;

		int m_voxel_grid_resolution;

		// OpenGL 3D Texture to store voxels in regular grid
		// if RegularGridTex3D enabled, stores result of latest call to Voxelizer::Voxelize()
		Tex3D m_voxels_tex3D;

		// Octree data structure for more efficient voxel storage
		// if OctreeHierarchy enabled, stores result of latest call to Voxelizer::Voxelize()
		//VoxelOctree m_voxels_octree;

		// VOXEL RAYCAST

		// shaders to draw voxels using raycasting
		Shader m_voxel_raycast_shader;
		Shader m_voxel_raycast_volumeexitposmap_shader;

		FrameBufferObject m_voxel_raycast_result_buffer;
		FrameBufferObject m_voxel_raycast_volumeexitposmap_buffer;

		float m_raycast_step_size;

		std::unique_ptr<Model> m_unitCube;

	public:
	
		Voxelizer();
		~Voxelizer();

		// voxelize the given mesh and store the result in a data structure
		// the resulting data structure is determined by m_voxel_storage_mode
		void Voxelize(std::unique_ptr<Model> &sourceMeshModel, int voxelGridResolution);

		// visualize the voxels
		void RenderVoxelGrid();
	};

}
