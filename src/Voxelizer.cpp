#include "Voxelizer.h"

namespace e186
{

    Voxelizer::Voxelizer()
	    : m_tweak_bar(Engine::current()->tweak_bar_manager().create_new_tweak_bar("Voxelizer"))
	    , m_voxel_grid_resolution(128)
	    , m_voxel_raycast_volumeexitposmap_buffer { Engine::current()->window_width(), Engine::current()->window_height() }
	{

		m_voxels_tex3D.GenerateEmpty(m_voxel_grid_resolution, m_voxel_grid_resolution, m_voxel_grid_resolution);
		m_voxels_tex3D.GenerateDepthTestData(m_voxel_grid_resolution, m_voxel_grid_resolution, m_voxel_grid_resolution);

		// BUILD SHADER PROGRAMS

		m_mesh_to_voxel_rasterization_shader
			.AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Geometry | ShaderType::Fragment)
			.AddVertexShaderSourceFromFile("assets/shaders/voxelize.vert")
			.AddGeometryShaderSourceFromFile("assets/shaders/voxelize.geom")
			.AddFragmentShaderSourceFromFile("assets/shaders/voxelize.frag")
			.Build();

		m_voxel_raycast_shader
		    .AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
		    .AddVertexShaderSourceFromFile("assets/shaders/voxel_raycast.vert")
		    .AddFragmentShaderSourceFromFile("assets/shaders/voxel_raycast.frag")
		    .Build();

		m_voxel_raycast_volumeexitposmap_shader
		    .AddToMultipleShaderSources(Shader::version_string(), ShaderType::Vertex | ShaderType::Fragment)
		    .AddVertexShaderSourceFromFile("assets/shaders/voxel_raycast_volumeexitposmap.vert")
		    .AddFragmentShaderSourceFromFile("assets/shaders/voxel_raycast_volumeexitposmap.frag")
		    .Build();

		// SETUP RAYCAST STUFF

		m_voxel_raycast_volumeexitposmap_buffer.AttachComponent(FboAttachmentConfig::kPresetRGB32F, GL_COLOR_ATTACHMENT0, TexParams::NearestFiltering);
		m_voxel_raycast_volumeexitposmap_buffer.AttachComponent(FboAttachmentConfig::kPresetDepthStencil24_8, { GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT }, TexParams::NearestFiltering);
		m_voxel_raycast_volumeexitposmap_buffer.set_clear_color(glm::vec4(.0f, .0f, .0f, 1.f));
		m_voxel_raycast_volumeexitposmap_buffer.Bind();
		m_voxel_raycast_volumeexitposmap_buffer.Clear();
		m_voxel_raycast_volumeexitposmap_buffer.Unbind();
		assert(m_voxel_raycast_volumeexitposmap_buffer.ready_for_action());

		// unit cube moved to be centered at origin
		m_unitCube = Model::LoadFromFile("assets/models/cube.obj", glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f)), MOLF_default);
		assert(m_unitCube);
		m_unitCube->CreateAndUploadGpuData();

		// SETUP ANTTWEAKBAR

		TwDefine("'Voxelizer' color='26 27 61' text=light position='50 50' ");

		TwEnumVal voxelStorageModeEV[] = {
		    { static_cast<int>(VoxelStorageMode::Tex3D), "Tex3D" },
		    { static_cast<int>(VoxelStorageMode::OctreeHierarchy), "OctreeHierarchy" }
		};
		TwType voxelStorageModeTWType = TwDefineEnum("Voxel Storage Mode", voxelStorageModeEV, 2);
		TwAddVarRW(m_tweak_bar, "Voxel Storage Mode", voxelStorageModeTWType, &m_voxel_storage_mode, "");
	}

	Voxelizer::~Voxelizer()
	{
	}

	void Voxelizer::Voxelize(std::unique_ptr<Model>& sourceMeshModel, int voxelGridResolution)
	{
		// TODO IMPLEMENT
		std::cout << "Voxelizer::Voxelize(const Model &sourceMeshModel) not yet fully implemented." << std::endl;

		m_voxel_grid_resolution = voxelGridResolution;

		// SETUP TARGET DATA STRUCTURES

		m_voxels_tex3D.GenerateEmpty(m_voxel_grid_resolution, m_voxel_grid_resolution, m_voxel_grid_resolution);

		// SETUP SHADER

		// predefine axis-aligned orthograhic view-projection matrices for geometry shader
		// note glm::ortho, i.e. the 'classical' orthographic projection matrix just maps to normalized device coords
		// i.e. scales and translates a defined cube of clipping planes to the unit cube (no perspective divide needed)
		// the actual 'flattening' is done by the hardware since result is a 2D buffer, z value ends up in zbuffer
		// we use three different with view matrices looking at unit cube from the three coordinate axes
		glm::mat4 orthoProjMat = glm::ortho<float>(-1, 1, -1, 1); // left, right, bottom, top clip planes
		glm::mat4 viewProjMatOrthoX = orthoProjMat * glm::lookAt<float>(glm::vec3(2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // eye, target, up vector
		glm::mat4 viewProjMatOrthoY = orthoProjMat * glm::lookAt<float>(glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1)); // up vector shouldnt be parallel to eye
		glm::mat4 viewProjMatOrthoZ = orthoProjMat * glm::lookAt<float>(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		m_mesh_to_voxel_rasterization_shader.Use();
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoX", viewProjMatOrthoX);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoY", viewProjMatOrthoY);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uViewProjMatOrthoZ", viewProjMatOrthoZ);
		m_mesh_to_voxel_rasterization_shader.SetUniform("uVoxelGridResolution", static_cast<int>(m_voxel_grid_resolution));
		m_mesh_to_voxel_rasterization_shader.SetImageTexture("uVoxelDiffuseReflectivity", m_voxels_tex3D, 0, 0, false, 0, GL_WRITE_ONLY);
		m_mesh_to_voxel_rasterization_shader.SetImageTexture("uVoxelNormal", m_voxels_tex3D, 1, 0, false, 0, GL_WRITE_ONLY);

		// select meshes to render
		auto meshes = sourceMeshModel->SelectAllMeshes();
		// generate uniform setters for selected meshes for a specific shader
		auto unisetters = Model::CompileUniformSetters(m_mesh_to_voxel_rasterization_shader, meshes);
		// get VAOs of all selected meshes
		auto render_data = Model::GetOrCreateRenderData(m_mesh_to_voxel_rasterization_shader, meshes);

		// DRAW

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// disable unwanted OpenGL functions
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // dont write to framebuffer, we use image load/store instead
		glDisable(GL_CULL_FACE); // we dont want to discard triangles facing a certain direction
		glDisable(GL_DEPTH_TEST); // we dont want to discard fragments that are behind others

		// set the viewport pixel width and height to the size of the voxel grid
		glViewport(0, 0, m_voxel_grid_resolution, m_voxel_grid_resolution);

		RenderMeshesWithAlignedUniformSetters(m_mesh_to_voxel_rasterization_shader, render_data, unisetters);

		UnbindVAO();
	}

	void Voxelizer::RenderVoxelGrid()
	{
		// TODO IMPLEMENT
		std::cout << "Voxelizer::RenderVoxelGrid() not yet fully implemented." << std::endl;

		glViewport(0, 0, Engine::current()->window_width(), Engine::current()->window_height());
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		auto& cube_mesh = m_unitCube->mesh_at(0);

		///////////////////////////////////////////////////////////////////////////////
		// FIRST PASS
		// generate ray volume exit position map later used to construct rays
		///////////////////////////////////////////////////////////////////////////////

		m_voxel_raycast_volumeexitposmap_buffer.Bind();

		m_voxel_raycast_volumeexitposmap_shader.Use();
		m_voxel_raycast_shader.SetUniform("uModelViewProjMat", glm::mat4(1.0f));

		// draw volume cube back faces (front face culling enabled)
		// rayVolumeExitPosMapShader stores interpolated back face (ray exit) positions in framebuffer texture
		glCullFace(GL_FRONT);
		RenderMesh(m_voxel_raycast_volumeexitposmap_shader, cube_mesh);

		///////////////////////////////////////////////////////////////////////////////
		// SECOND PASS
		// calculate ray volume entry positions and together with exit position map
		// do raycasting from entry to exit position of each fragment
		///////////////////////////////////////////////////////////////////////////////

		FrameBufferObject::default_framebuffer().Bind().ClearColor().ClearDepth().SetViewport();

		m_voxel_raycast_shader.Use();
		m_voxel_raycast_shader.SetUniform("uModelViewProjMat", glm::mat4(1.0f));
		m_voxel_raycast_shader.SetUniform("uRaycastStepSize", m_raycast_step_size);
		const auto* raycast_volumeexitposmap_tex = m_voxel_raycast_volumeexitposmap_buffer.FindAttachedTexture(GL_COLOR_ATTACHMENT0);
		m_voxel_raycast_shader.SetImageTexture("uExitPositions", raycast_volumeexitposmap_tex, 0, 0, false, 0, GL_READ_ONLY);
		m_voxel_raycast_shader.SetImageTexture("uVoxelDiffuseReflectivity", m_voxels_tex3D, 1, 0, false, 0, GL_READ_ONLY);

		// draw unit cube with back face culling to get front face fragments where view rays enter volume
		// raycastShader uses ray entry positions with exit positions from first pass
		// these rays through the unit cube are used for 3D texture sampling
		// raycasting then steps along the ray and samples the voxel intensities and outputs a color
		glCullFace(GL_BACK);
		RenderMesh(m_voxel_raycast_shader, cube_mesh);
		glDisable(GL_CULL_FACE);
		UnbindVAO();

		// DEBUG VIEW FIRST PASS TEXTURE
		// blit framebuffer from first pass to default framebuffer
		// blit = bit block image transfer, combine bitmaps via boolean operation
		//BlitColor(m_voxel_raycast_volumeexitposmap_buffer, FrameBufferObject::default_framebuffer());

		std::cout << "DEBUG working until here" << std::endl;


	}


}
