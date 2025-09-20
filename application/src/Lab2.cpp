#include "Lab2.hpp"
#include "scripts/include/camera.hpp"
#include "scripts/include/rotation.hpp"
#include <numeric> // For std::iota
#include <glm/gtc/matrix_transform.hpp>




Lab2::Lab2(GLFWWindowImpl& win) : Layer(win)
{
	//scenes
	m_mainScene.reset(new Scene);
	

	float width = m_winRef.getWidthf();
	float height = m_winRef.getHeightf();
	const vector<float> screenVertices = {
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //bottom left corner
	width, 0.0f, 0.0f, 1.0f, 1.0f, //bottom right corner
	width, height, 0.0f, 1.0f, 0.0f, //top right corner
	0.0f, height, 0.0f, 0.0f, 0.0f //top left corner
	}; 
	const vector<uint32_t> screenIndices = { 0,1,2,2,3,0 };
	float near = 0.1f;
	float far = 1000.f;

	//billboards
	//positions
	
	vector<float> billboardPositions;

	for (int i = 0; i < 5; i++) {
		billboardPositions.push_back(Randomiser::uniformFloatBetween(-50.f, 50.f));//x
		billboardPositions.push_back(0.0f);//y
		billboardPositions.push_back(Randomiser::uniformFloatBetween(-30.f, 35.f));//z

	}
	//shader description
	ShaderDescription billboardShaderDesc;
	billboardShaderDesc.type = ShaderType::geometry;
	billboardShaderDesc.vertexSrcPath = "./assets/shaders/bbShader/billboardVert.glsl";
	billboardShaderDesc.geometrySrcPath = "./assets/shaders/bbShader/billboardGeo.glsl";
	billboardShaderDesc.fragmentSrcPath = "./assets/shaders/bbShader/billboardFrag.glsl";
	std::shared_ptr<Shader> billboardShader;
	billboardShader = std::make_shared<Shader>(billboardShaderDesc);

	vector<uint32_t> billBoardIndices(billboardPositions.size());
	std::iota(billBoardIndices.begin(), billBoardIndices.end(), 0);
	shared_ptr<VAO> billboardVAO;
	billboardVAO = make_shared<VAO>(billBoardIndices);
	billboardVAO->addVertexBuffer(billboardPositions, { {GL_FLOAT, 3} });
	
	shared_ptr<Material> billboardMatirial = make_shared<Material>(billboardShader);
	std::shared_ptr<Texture> billboardTexture = std::make_shared<Texture>("./assets/textures/tree.png");
	billboardMatirial->setPrimitive(GL_POINTS);
	billboardMatirial->setValue("u_billboardImage", billboardTexture);
	
	Actor bb;
	bb.geometry = billboardVAO;
	bb.material = billboardMatirial;

	bbIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(bb);







	//creating a vampire shader
	ShaderDescription cubephongShaderDesc;
	cubephongShaderDesc.type = ShaderType::rasterization;
	cubephongShaderDesc.vertexSrcPath = "./assets/shaders/modelShader/PhongVert.glsl";
	cubephongShaderDesc.fragmentSrcPath = "./assets/shaders/modelShader/PhongFrag.glsl";
	//making a vampire
	std::shared_ptr<Shader> cubephongShader;
	cubephongShader = std::make_shared<Shader>(cubephongShaderDesc);
	VBOLayout cubeLayout = {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 3},  // normal
		{GL_FLOAT, 2},  // UV
		{GL_FLOAT, 3}   // tan

	};
	VBOLayout modelDepthLayout = { {GL_FLOAT, 3 } };

	uint32_t attributeTypes = Model::VertexFlags::positions |
		Model::VertexFlags::normals |
		Model::VertexFlags::uvs |
		Model::VertexFlags::tangents;

	//reading in the vampire model with Assmip
	Model cubeModel("./assets/models/FuturisticSoldier/Mesh/OBJ/Futuristic_Soldier.obj", attributeTypes);
	std::shared_ptr<VAO> cubeVAO;
	cubeVAO = std::make_shared<VAO>(cubeModel.m_meshes[0].indices);
	cubeVAO->addVertexBuffer(cubeModel.m_meshes[0].vertices, cubeLayout);
	//depth VAO
	
	std::shared_ptr<VAO> depthModelVAO;
	depthModelVAO = std::make_shared<VAO>(cubeModel.m_meshes[0].indices);
	depthModelVAO->addVertexBuffer(cubeModel.m_meshes[0].positions, modelDepthLayout);



	//depth
	ShaderDescription depthModelShaderDesc;
	depthModelShaderDesc.type = ShaderType::rasterization;
	depthModelShaderDesc.vertexSrcPath = "./assets/shaders/modelShader/shadowVert.glsl";
	depthModelShaderDesc.fragmentSrcPath = "./assets/shaders/modelShader/shadowFrag.glsl";
	//making a vampire
	std::shared_ptr<Shader> depthModelShader;
	depthModelShader = std::make_shared<Shader>(depthModelShaderDesc);
	//creating the vampire material
	std::shared_ptr<Material> depthModelMaterial;
	depthModelMaterial = std::make_shared<Material>(depthModelShader);

	std::shared_ptr<Texture> modelDiffuse;
	modelDiffuse = std::make_shared<Texture>("./assets/models/FuturisticSoldier/Textures PBR/Green/greenGuy.png");
	std::shared_ptr<Texture> modelNormal;
	modelNormal = std::make_shared<Texture>("./assets/models/FuturisticSoldier/Textures PBR/Green/greenNormal.png");
	std::shared_ptr<Texture> modelMetal;
	modelMetal = std::make_shared<Texture>("./assets/models/FuturisticSoldier/Textures PBR/Green/greenMetal2.png");
	std::shared_ptr<Texture> modelRough;
	modelRough = std::make_shared<Texture>("./assets/models/FuturisticSoldier/Textures PBR/Green/greenRough.png");

	std::shared_ptr<Material> cubeMaterial;
	cubeMaterial = std::make_shared<Material>(cubephongShader);
	
	cubeMaterial->setValue("u_albedoMap", modelDiffuse);
	cubeMaterial->setValue("u_normalMap", modelNormal);
	cubeMaterial->setValue("u_metalMap", modelMetal);
	cubeMaterial->setValue("u_roughMap", modelRough);
	Actor vampire;
	for (int i = 0; i < vampirePos.size(); i++) {
		vampire.geometry = cubeVAO;
		vampire.depthGeometry = depthModelVAO;
		vampire.material = cubeMaterial;
		vampire.depthMaterial = depthModelMaterial;
		vampire.translation = glm::vec3(vampirePos[i]);
		vampire.scale = glm::vec3(5.f, 5.f, 5.f);
		vampire.recalc();
		modelIdx = m_mainScene->m_actors.size();
		m_mainScene->m_actors.push_back(vampire);

	}


	


	//adding point lights
	PointLight pointLight;
	uint32_t numPointLights = 50;
	for (int i = 0; i < numPointLights; i++) {
		pointLight.colour = glm::vec3(Randomiser::uniformFloatBetween(0.0, 1.0), Randomiser::uniformFloatBetween(0.0, 1.0), Randomiser::uniformFloatBetween(0.0, 1.0));
		pointLight.position = glm::vec3(Randomiser::uniformFloatBetween(-30.0, 30.0), 5.f, Randomiser::uniformFloatBetween(-30.0, 30.0));
		pointLight.constants = glm::vec3(1.0f, 0.09f, 0.032f);
		m_mainScene->m_pointLights.push_back(pointLight);
	}
	//adding directional light
	
	dl.direction = lightDir;
	m_mainScene->m_directionalLights.push_back(dl);
	//adding a camera
	Actor camera;
	cameraIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(camera);

	//adding a skybox
		//creating a skybox shader

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//particles
	
	ShaderDescription partShaderDesc;
	partShaderDesc.type = ShaderType::rasterization;
	partShaderDesc.vertexSrcPath = "./assets/shaders/compute/partVert.glsl";
	partShaderDesc.fragmentSrcPath = "./assets/shaders/compute/partFrag.glsl";
	std::shared_ptr<Shader> partShader;
	partShader = std::make_shared<Shader>(partShaderDesc);

	shared_ptr<Material> partMatierial = make_shared<Material>(partShader);
	partMatierial->setPrimitive(GL_POINTS);

	vector<uint32_t> partIndices(particleAmount);
	iota(partIndices.begin(), partIndices.end(), 0);

	shared_ptr<VAO> partVAO;
	partVAO = make_shared<VAO>(partIndices);

	Actor partActor;
	partActor.geometry = partVAO;
	partActor.material = partMatierial;
	partActor.recalc();
	particlesIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(partActor);
	
	
	
	


	//////////////////////////////////////////////////////////////////////////
	FBOLayout typicalLayout = {
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};
	FBOLayout visaulDepthLayout = {
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, false}
	};
	FBOLayout shadowLayout = {
		{AttachmentType::Depth, true}
	};
	FBOLayout GBuffer = {
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::ColourHDR, true},
		{AttachmentType::Depth, true}
	};

	///////////////////////////////////////////////////////////////////////////////////////




	//////////////////////////////////////////////////////////////////////////////////////
	DepthPass shadowPass;
	shadowPass.scene = m_mainScene;
	shadowPass.parseScene();
	shadowPass.target = std::make_shared<FBO>(glm::ivec2(4096, 4096), shadowLayout);
	shadowPass.viewPort = { 0, 0, 4096,4096 };

	lightDir = dl.direction;
	glm::vec3 lightPosition = (m_shadowMapVars.centre - lightDir) * m_shadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_shadowMapVars.centre, m_shadowMapVars.Up);
	shadowPass.camera.view = shadowView;

	shadowPass.camera.projection = glm::ortho(-m_shadowMapVars.orthoSize,
		m_shadowMapVars.orthoSize,
		-m_shadowMapVars.orthoSize,
		m_shadowMapVars.orthoSize,
		-m_shadowMapVars.orthoSize / 2,
		m_shadowMapVars.orthoSize * 2);
	shadowPass.UBOmanager.setCachedValue("b_lightcamera", "u_view", shadowPass.camera.view);
	shadowPass.UBOmanager.setCachedValue("b_lightcamera", "u_projection", shadowPass.camera.projection);
	depthIdx = m_mainRenderer.getDepthPassCount();
	m_mainRenderer.addDepthPass(shadowPass);

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TextureDescription computeDesc;
	computeDesc.width = 512;
	computeDesc.height = 512;
	computeDesc.channels = 4;
	computeDesc.isHDR = false;

	shared_ptr<Texture> emptyTexture = make_shared<Texture>(computeDesc);

	ShaderDescription noiseComputeDesc;
	noiseComputeDesc.type = ShaderType::compute;
	noiseComputeDesc.computeSrcPath = "./assets/shaders/compute/computeShader.glsl";
	std::shared_ptr<Shader> noiseComputeShader;
	noiseComputeShader = std::make_shared<Shader>(noiseComputeDesc);

	shared_ptr<Material> noisecomputeMatierial = make_shared<Material>(noiseComputeShader);

	noisecomputeMatierial->setValue("u_octaves", float(octaves));
	noisecomputeMatierial->setValue("u_amplitude", amplitude);
	noisecomputeMatierial->setValue("u_frequency", frequency);
	noisecomputeMatierial->setValue("u_persitance", persistance);
	noisecomputeMatierial->setValue("u_lacunarity", lacunarity);


	ComputePass noiseComputePass;
	noiseComputePass.material = noisecomputeMatierial;
	noiseComputePass.workgroups = { 32,32,1 };
	noiseComputePass.barrier = MemoryBarrier::ShaderImageAccess;
	//creating the image
	Image noiseComputeImage;
	noiseComputeImage.mipLevel = 0;
	noiseComputeImage.layered = false;
	noiseComputeImage.texture = emptyTexture;
	noiseComputeImage.imageUnit = noiseComputePass.material->m_shader->m_imageBindingPoints["outputImage"];
	noiseComputeImage.access = TextureAccess::WriteOnly;
	noiseComputePass.images.push_back(noiseComputeImage);

	computeIdx = m_mainRenderer.getComputePassCount();
	m_mainRenderer.addComputePass(noiseComputePass);

	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	shared_ptr<Texture> emptyTexture2 = make_shared<Texture>(computeDesc);

	ShaderDescription textureComputeDesc;
	textureComputeDesc.type = ShaderType::compute;
	textureComputeDesc.computeSrcPath = "./assets/shaders/compute/CDMcomputeShader.glsl";
	std::shared_ptr<Shader> computeShader;
	computeShader = std::make_shared<Shader>(textureComputeDesc);

	shared_ptr<Material> computeMatierial = make_shared<Material>(computeShader);

	computeMatierial->setValue("u_heightMap", emptyTexture);

	ComputePass textureComputePass;
	textureComputePass.material = computeMatierial;
	textureComputePass.workgroups = { 32,32,1 };
	textureComputePass.barrier = MemoryBarrier::ShaderImageAccess;
	//creating the image
	Image computeImage;
	computeImage.mipLevel = 0;
	computeImage.layered = false;
	computeImage.texture = emptyTexture2;
	computeImage.imageUnit = textureComputePass.material->m_shader->m_imageBindingPoints["outputImage"];
	computeImage.access = TextureAccess::WriteOnly;
	textureComputePass.images.push_back(computeImage);

	textureComputeIdx = m_mainRenderer.getComputePassCount();
	m_mainRenderer.addComputePass(textureComputePass);

	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Image Based Lighting
	/*
	ShaderDescription IBLComputeDesc;
	IBLComputeDesc.type = ShaderType::compute;
	IBLComputeDesc.computeSrcPath = "./assets/shaders/compute/IBLcomputeShader.glsl";
	std::shared_ptr<Shader> IBLcomputeShader;
	IBLcomputeShader = std::make_shared<Shader>(IBLComputeDesc);

	shared_ptr<Material> IBLcomputeMatierial = make_shared<Material>(IBLcomputeShader);
	//IBLcomputeMatierial->setValue("equirectangularMap", );

	ComputePass IBLComputePass;
	IBLComputePass.material = computeMatierial;
	IBLComputePass.workgroups = { 32,32,1 };
	IBLComputePass.barrier = MemoryBarrier::ShaderImageAccess;
	//creating the image
	Image IBLcomputeImage;
	IBLcomputeImage.mipLevel = 0;
	IBLcomputeImage.layered = false;
	IBLcomputeImage.texture = emptyTexture2;
	IBLcomputeImage.imageUnit = IBLComputePass.material->m_shader->m_imageBindingPoints["outputImage"];
	IBLcomputeImage.access = TextureAccess::WriteOnly;
	IBLComputePass.images.push_back(IBLcomputeImage);

	textureComputeIdx = m_mainRenderer.getComputePassCount();
	m_mainRenderer.addComputePass(IBLComputePass);
	*/
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//particle update compute shader
	//Initialise
	vector<Particle> particles(particleAmount);
	for (int i = 0; i < particles.size(); i++) {
		particles[i].position = glm::vec4(Randomiser::uniformFloatBetween(-30.0, 30.0), Randomiser::uniformFloatBetween(-30.0, 30.0), Randomiser::uniformFloatBetween(-30.0, 30.0), 1.0);
		particles[i].velocity = glm::vec4(
			Randomiser::uniformFloatBetween(-0.2, 0.2),  // slight horizontal drift
			Randomiser::uniformFloatBetween(-2.0, -0.5), // falling down in Y
			Randomiser::uniformFloatBetween(-0.2, 0.2),  // slight depth drift
			0.0f
		);
		//particles[i].age = Randomiser::uniformFloatBetween(3.0, 10.0);
	}
	shared_ptr<SSBO> particleSSBO = make_shared<SSBO>(sizeof(Particle) * particleAmount, particleAmount, particles.data());
	
	//update
	ShaderDescription particleUpdateComputeDesc;
	particleUpdateComputeDesc.type = ShaderType::compute;
	particleUpdateComputeDesc.computeSrcPath = "./assets/shaders/compute/particleUpdateComputeShader.glsl";
	std::shared_ptr<Shader> particleUpdateComputeShader;
	particleUpdateComputeShader = std::make_shared<Shader>(particleUpdateComputeDesc);

	shared_ptr<Material> particleUpdateComputeMatierial = make_shared<Material>(particleUpdateComputeShader);


	ComputePass particleUpdateComputePass;
	particleUpdateComputePass.material = particleUpdateComputeMatierial;
	particleUpdateComputePass.workgroups = { 32, 32, 1 };
	particleUpdateComputePass.barrier = MemoryBarrier::ShaderStorageAccess;
	particleUpdateComputePass.ssbo = particleSSBO;

	ParticleComputeIdx = m_mainRenderer.getComputePassCount();
	m_mainRenderer.addComputePass(particleUpdateComputePass);
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//creating a floor shader
	ShaderDescription floorphongShaderDesc;
	floorphongShaderDesc.type = ShaderType::tessellationAndGeometry;
	floorphongShaderDesc.vertexSrcPath = "./assets/shaders/floorShader/floorVertShader.glsl";
	floorphongShaderDesc.controlSrcPath = "./assets/shaders/floorShader/floorTcs.glsl";
	floorphongShaderDesc.evaluationSrcPath = "./assets/shaders/floorShader/floorTES.glsl";
	floorphongShaderDesc.geometrySrcPath = "./assets/shaders/floorShader/floorGeo.glsl";
	floorphongShaderDesc.fragmentSrcPath = "./assets/shaders/floorShader/floorFragShader.glsl";
	//making a floor
	std::shared_ptr<Shader> floorphongShader;
	floorphongShader = std::make_shared<Shader>(floorphongShaderDesc);
	VBOLayout floorLayout = {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
	};
	//floor data
	std::shared_ptr<Grid> grid = make_shared<Grid>();
	std::vector<float> floor_vertices = grid->getVertices();
	std::vector<unsigned int> floor_indices = grid->getIndices();
	//reading in the floor model with Assmip
	std::shared_ptr<VAO> floorVAO;
	floorVAO = std::make_shared<VAO>(floor_indices);
	floorVAO->addVertexBuffer(floor_vertices, floorLayout);
	//depth vao
	//creating the floor material
	std::shared_ptr<Texture> floorGrassTexture;
	std::shared_ptr<Texture> floorGrassMetalicMap;
	std::shared_ptr<Texture> floorGrassRoughMap;
	floorGrassTexture = std::make_shared<Texture>("./assets/textures/grass/grassAlbedo.png");
	floorGrassMetalicMap = std::make_shared<Texture>("./assets/textures/grass/grassMetallic.png");
	floorGrassRoughMap = std::make_shared<Texture>("./assets/textures/grass/grassRoughness.png");

	std::shared_ptr<Texture> floorRockTexture;
	std::shared_ptr<Texture> floorRockMetalicMap;
	std::shared_ptr<Texture> floorRockRoughMap;
	floorRockTexture = std::make_shared<Texture>("./assets/textures/stone/rockAlbedo.png");
	floorRockMetalicMap = std::make_shared<Texture>("./assets/textures/stone/rockMetallic2.png");
	floorRockRoughMap = std::make_shared<Texture>("./assets/textures/stone/rockRoughness2.png");

	std::shared_ptr<Texture> floorHeightMap;
	std::shared_ptr<Texture> floorNormalMap;
	floorHeightMap = emptyTexture;
	floorNormalMap = emptyTexture2;


	std::shared_ptr<Material> floorMaterial;
	floorMaterial = std::make_shared<Material>(floorphongShader);
	floorMaterial->setPrimitive(GL_PATCHES);
	//tessalation
	floorMaterial->setValue("value1", value1);
	floorMaterial->setValue("value2", value2);
	//gras texture
	floorMaterial->setValue("u_albedoMap", floorGrassTexture);
	floorMaterial->setValue("u_GrassMetalMap", floorGrassMetalicMap);
	floorMaterial->setValue("u_GrassRoughMap", floorGrassRoughMap);
	//rock texture
	floorMaterial->setValue("u_rockTex", floorRockTexture);
	floorMaterial->setValue("u_RockMetalMap", floorRockMetalicMap);
	floorMaterial->setValue("u_RockRoughMap", floorRockRoughMap);
	
	floorMaterial->setValue("u_heightMap", floorHeightMap);
	floorMaterial->setValue("u_normalMap", floorNormalMap);
	//floorMaterial->setValue("u_albedo", m_colour);
	Actor floor;
	floor.geometry = floorVAO;
	floor.material = floorMaterial;
	floor.translation = glm::vec3(-50.f, -5.f, -50.f);
	floor.recalc();
	floorIdx = m_mainScene->m_actors.size();
	m_mainScene->m_actors.push_back(floor);


	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*************************
*  Main Render Pass
**************************/
//drawing to the screen
	RenderPass mainPass;
	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>(m_winRef.getSize(), GBuffer);
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 1000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	mainPass.camera.updateView(m_mainScene->m_actors.at(cameraIdx).transform);
	//setting UBO(Uniform Buffer Object) values. Setting values in one place to be used everywhere in the code.
	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	
	
 
	
	//m_mainScene->m_actors.at(modelIdx).attachScript<RotationScript>(mainPass.scene->m_actors.at(modelIdx), glm::vec3(0.3f, 0.6f, 0.9f), GLFW_KEY_1);
	mainIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(mainPass);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//rendering the skybox
	ShaderDescription skyboxphongShaderDesc;
	skyboxphongShaderDesc.type = ShaderType::rasterization;
	skyboxphongShaderDesc.vertexSrcPath = "./assets/shaders/skybox/skyboxVert.glsl";
	skyboxphongShaderDesc.fragmentSrcPath = "./assets/shaders/skybox/skyboxFrag.glsl";
	std::shared_ptr<Shader> skyboxphongShader;
	skyboxphongShader = std::make_shared<Shader>(skyboxphongShaderDesc);

	//skybox data
	std::vector<uint32_t> skyboxIndices(skyboxVertices.size() / 3);
	std::iota(skyboxIndices.begin(), skyboxIndices.end(), 0);

	std::shared_ptr<VAO> skyboxVAO;
	skyboxVAO = std::make_shared<VAO>(skyboxIndices);
	skyboxVAO->addVertexBuffer(skyboxVertices, { {GL_FLOAT, 3} });

	std::array<const char*, 6> skyboxMapPaths = {
		"./assets/textures/Skybox/right.png",
		"./assets/textures/Skybox/left.png",
		"./assets/textures/Skybox/top.png",
		"./assets/textures/Skybox/bottom.png",
		"./assets/textures/Skybox/back.png",
		"./assets/textures/Skybox/front.png"
	};
	std::shared_ptr<CubeMap> skyboxMap;
	skyboxMap = std::make_shared<CubeMap>(skyboxMapPaths, false, false);

	std::shared_ptr<Material> skyboxMaterial;
	skyboxMaterial = std::make_shared<Material>(skyboxphongShader);

	//setting actor
	Actor skybox;
	skybox.geometry = skyboxVAO;
	skybox.material = skyboxMaterial;
	skyboxMaterial->setValue("u_skyboxView", glm::inverse(skybox.transform));
	skyboxMaterial->setValue("u_skybox", skyboxMap);
	skyboxIdx = m_mainScene->m_actors.size();
	m_screenScene.reset(new Scene);
	m_mainScene->m_actors.push_back(skybox);

	RenderPass skyboxPass;
	skyboxPass.scene = m_screenScene;
	skyboxPass.parseScene();
	skyboxPass.target = make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	skyboxPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };
	
	
	
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//light pass
	
	ShaderDescription lightShaderDesc;
	lightShaderDesc.type = ShaderType::rasterization;
	lightShaderDesc.vertexSrcPath = "./assets/shaders/postPro/LightVert.glsl";
	lightShaderDesc.fragmentSrcPath = "./assets/shaders/postPro/LightFrag.glsl";
	std::shared_ptr<Shader> lightShader;
	lightShader = std::make_shared<Shader>(lightShaderDesc);
	VBOLayout QuadLayout = {
		{GL_FLOAT, 3},  // position
		{GL_FLOAT, 2}   // UV
	};
	shared_ptr<VAO> QuadVAO;
	QuadVAO = make_shared<VAO>(screenIndices);
	QuadVAO->addVertexBuffer(screenVertices, QuadLayout);

	shared_ptr<Material> lightMatierial = make_shared<Material>(lightShader);
	lightMatierial->setValue("gBufferPosWS", mainPass.target->getTarget(0));
	lightMatierial->setValue("gBufferNormals", mainPass.target->getTarget(1));
	lightMatierial->setValue("gBufferDiffSpec", mainPass.target->getTarget(2));
	lightMatierial->setValue("gRoughness", mainPass.target->getTarget(3));
	lightMatierial->setValue("gBufferDepth", mainPass.target->getTarget(4));
	lightMatierial->setValue("skyBox", skyboxPass.target->getTarget(0));

	Actor Quad;
	Quad.geometry = QuadVAO;
	Quad.material = lightMatierial;
	m_screenScene.reset(new Scene);
	m_screenScene->m_actors.push_back(Quad);
	
	RenderPass lightPass;
	lightPass.scene = m_screenScene;
	lightPass.parseScene();
	lightPass.target = make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	lightPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };

	lightPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	lightPass.UBOmanager.setCachedValue("b_camera2D", "u_view", lightPass.camera.view);
	lightPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", lightPass.camera.projection);
	lightPass.UBOmanager.setCachedValue("b_lights", "u_viewPos", m_mainScene->m_actors.at(cameraIdx).translation);
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	lightPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);
	for (int i = 0; i < numPointLights; i++) {
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].colour", m_mainScene->m_pointLights.at(i).colour);
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].position", m_mainScene->m_pointLights.at(i).position);
		lightPass.UBOmanager.setCachedValue("b_lights", "pLights[" + std::to_string(i) + "].constants", m_mainScene->m_pointLights.at(i).constants);
	}
	LightIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(lightPass);

	lightMatierial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	//cubeMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);

	//floorMaterial->setValue("u_shadowMap", shadowPass.target->getTarget(0));
	lightMatierial->setValue("u_shadowMap", shadowPass.target->getTarget(0));








	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//fog pass
	ShaderDescription fogShaderDesc;
	fogShaderDesc.type = ShaderType::rasterization;
	fogShaderDesc.vertexSrcPath = "./assets/shaders/postPro/invertVert.glsl";
	fogShaderDesc.fragmentSrcPath = "./assets/shaders/postPro/fogFrag.glsl";
	std::shared_ptr<Shader> fogShader;
	fogShader = std::make_shared<Shader>(fogShaderDesc);

	//material
	shared_ptr<Material> fogQuadMaterial;
	fogQuadMaterial = make_shared<Material>(fogShader);
	fogQuadMaterial->setValue("u_inputTexture", lightPass.target->getTarget(0));
	fogQuadMaterial->setValue("u_depthTex", lightPass.target->getTarget(1));
	fogQuadMaterial->setValue("u_fogColour", glm::vec3(1.0f,1.0f,1.0f));

	Quad.material = fogQuadMaterial;
	m_screenScene.reset(new Scene);
	m_screenScene->m_actors.push_back(Quad);


	RenderPass fogPass;
	fogPass.scene = m_screenScene;
	fogPass.parseScene();
	fogPass.target = make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	fogPass.viewPort = { 0, 0, m_winRef.getWidth(),m_winRef.getHeight() };

	fogPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_view", fogPass.camera.view);
	fogPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", fogPass.camera.projection);
	fogIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(fogPass);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//chromatic aberation
	ShaderDescription chromaticShaderDesc;
	chromaticShaderDesc.type = ShaderType::rasterization;
	chromaticShaderDesc.vertexSrcPath = "./assets/shaders/postPro/invertVert.glsl";
	chromaticShaderDesc.fragmentSrcPath = "./assets/shaders/postPro/chromaticFrag.glsl";
	std::shared_ptr<Shader> chromaticShader;
	chromaticShader = std::make_shared<Shader>(chromaticShaderDesc);

	//material
	shared_ptr<Material> chromaticMaterial;
	chromaticMaterial = make_shared<Material>(chromaticShader);
	chromaticMaterial->setValue("u_inputTexture", fogPass.target->getTarget(0));
	chromaticMaterial->setValue("u_expoStrength", expoStrength);


	Quad.material = chromaticMaterial;
	m_screenScene.reset(new Scene);
	m_screenScene->m_actors.push_back(Quad);


	RenderPass chromaticPass;
	chromaticPass.scene = m_screenScene;
	chromaticPass.parseScene();
	chromaticPass.target = make_shared<FBO>(m_winRef.getSize(), typicalLayout);
	chromaticPass.viewPort = { 0, 0, m_winRef.getWidth(),m_winRef.getHeight() };

	chromaticPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	chromaticPass.UBOmanager.setCachedValue("b_camera2D", "u_view", chromaticPass.camera.view);
	chromaticPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", chromaticPass.camera.projection);
	chromaticIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(chromaticPass);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//visulise depth
	ShaderDescription visualDpethShaderDesc;
	visualDpethShaderDesc.type = ShaderType::rasterization;
	visualDpethShaderDesc.vertexSrcPath = "./assets/shaders/postPro/invertVert.glsl";
	visualDpethShaderDesc.fragmentSrcPath = "./assets/shaders/postPro/depthFrag.glsl";
	std::shared_ptr<Shader> visualDepthShader;
	visualDepthShader = std::make_shared<Shader>(visualDpethShaderDesc);

	//material
	shared_ptr<Material> visualDepthQuadMaterial;
	visualDepthQuadMaterial = make_shared<Material>(visualDepthShader);
	visualDepthQuadMaterial->setValue("u_inputTexture", shadowPass.target->getTarget(0));
	Quad.material = visualDepthQuadMaterial;
	m_screenScene.reset(new Scene);
	m_screenScene->m_actors.push_back(Quad);


	RenderPass visualDepthPass;
	visualDepthPass.scene = m_screenScene;
	visualDepthPass.parseScene();
	visualDepthPass.target = make_shared<FBO>(m_winRef.getSize(), visaulDepthLayout);
	visualDepthPass.viewPort = { 0, 0, m_winRef.getWidth(),m_winRef.getHeight() };

	visualDepthPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	visualDepthPass.UBOmanager.setCachedValue("b_camera2D", "u_view", visualDepthPass.camera.view);
	visualDepthPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", visualDepthPass.camera.projection);
	visualDepthIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(visualDepthPass);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //screenshader
	ShaderDescription screenShaderDesc;
	screenShaderDesc.type = ShaderType::rasterization;
	screenShaderDesc.vertexSrcPath = "./assets/shaders/postPro/screenVert.glsl";
	screenShaderDesc.fragmentSrcPath = "./assets/shaders/postPro/screenFrag.glsl";
	std::shared_ptr<Shader> screenShader;
	screenShader = std::make_shared<Shader>(screenShaderDesc);

	//material
	shared_ptr<Material> screenQuadMaterial;
	screenQuadMaterial = make_shared<Material>(screenShader);
	screenQuadMaterial->setValue("u_inputTexture", chromaticPass.target->getTarget(0));
	Quad.material = screenQuadMaterial;
	m_screenScene.reset(new Scene);
	m_screenScene->m_actors.push_back(Quad);


	RenderPass screenPass;
	screenPass.scene = m_screenScene;
	screenPass.parseScene();
	screenPass.target = make_shared<FBO>();
	screenPass.viewPort = { 0, 0, m_winRef.getWidth(),m_winRef.getHeight() };

	screenPass.camera.projection = glm::ortho(0.f, width, height, 0.f);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_view", screenPass.camera.view);
	screenPass.UBOmanager.setCachedValue("b_camera2D", "u_projection", screenPass.camera.projection);
	screenIdx = m_mainRenderer.getRenderPassCount();
	m_mainRenderer.addRenderPass(screenPass);

	//attaching camera scripts 
	m_mainScene->m_actors.at(cameraIdx).attachScript<CameraScript>(mainPass.scene->m_actors.at(cameraIdx), m_winRef, glm::vec3(1.6f, 0.6f, 2.f), 0.5f);
}



void Lab2::onRender() const
{
	//m_initialseRenderer.render();
	m_mainRenderer.render();
}

void Lab2::onUpdate(float timestep)
{

	// Update scripts
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onUpdate(timestep);
	}
	int addDepthPasses = m_mainRenderer.getDepthPassCount();
	int addComputePasses = m_mainRenderer.getComputePassCount();

	// Update camera  and its position in UBO
	auto& camera = m_mainScene->m_actors.at(cameraIdx);
	auto& pass = m_mainRenderer.getRenderPass(mainIdx + addDepthPasses + addComputePasses);
	pass.camera.updateView(camera.transform);
	pass.UBOmanager.setCachedValue("b_camera", "u_view", pass.camera.view);
	pass.UBOmanager.setCachedValue("b_camera", "u_viewPos", camera.translation);

	
	//auto& something = m_mainScene->m_actors.at(particlesIdx).material;
	//something->setValue("u_view", pass.camera.view);
	//something->setValue("u_projection", pass.camera.projection);

	//directional light

	//auto& cubeMaterial = m_mainScene->m_actors.at(modelIdx).material;
	auto& lightPass = m_mainRenderer.getRenderPass(2);
	auto& lightMaterial = lightPass.scene->m_actors.at(0).material;
	
	auto& shadowPass = m_mainRenderer.getDepthPass(0);
	lightDir = glm::normalize(dl.direction);
	
	glm::vec3 lightPosition = (m_shadowMapVars.centre - lightDir) * m_shadowMapVars.distanceAlongLightVector;
	glm::mat4 shadowView = glm::lookAt(lightPosition, m_shadowMapVars.centre, m_shadowMapVars.Up);
	shadowPass.camera.view = shadowView;
	shadowPass.camera.projection = glm::ortho(-m_shadowMapVars.orthoSize,
		m_shadowMapVars.orthoSize,
		-m_shadowMapVars.orthoSize,
		m_shadowMapVars.orthoSize,
		-m_shadowMapVars.orthoSize / 2,
		m_shadowMapVars.orthoSize * 2);
	shadowPass.UBOmanager.setCachedValue("b_lightcamera", "u_view", shadowPass.camera.view);
	shadowPass.UBOmanager.setCachedValue("b_lightcamera", "u_projection", shadowPass.camera.projection);

	lightMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);
	//cubeMaterial->setValue("u_lightSpaceTransform", shadowPass.camera.projection * shadowPass.camera.view);


	auto& skybox = m_mainScene->m_actors.at(skyboxIdx);
	skybox.material->setValue("u_skyboxView", glm::mat4(glm::mat3(pass.camera.view)));

	if (m_wireFrame)
	{
		m_mainRenderer.getRenderPass(0).drawInWireFrame = true;
	}
	else
	{
		m_mainRenderer.getRenderPass(0).drawInWireFrame = false;

	}
	//updating floor tess level
	auto& floorMat = m_mainRenderer.getRenderPass(mainIdx + addDepthPasses + addComputePasses).scene->m_actors.at(floorIdx).material;
	floorMat->setValue("value1", value1);
	floorMat->setValue("value2", value2);
	//changing fog colour
	auto& fogMat = m_mainRenderer.getRenderPass(fogIdx + addDepthPasses + addComputePasses).scene->m_actors.at(0).material;
	fogMat->setValue("u_fogColour", fogColour);
	fogMat->setValue("u_fogDensity", m_fogDensity);
	//chromatic aberration strength
	auto& chromeMat = m_mainRenderer.getRenderPass(chromaticIdx + addDepthPasses + addComputePasses).scene->m_actors.at(0).material;
	chromeMat->setValue("u_expoStrength", expoStrength);

	//setting noise values
	auto& NoiseMat = m_mainRenderer.getComputePass(computeIdx + addDepthPasses).material;
	NoiseMat->setValue("u_octaves", float(octaves));
	NoiseMat->setValue("u_frequency", frequency);
	NoiseMat->setValue("u_amplitude", amplitude);
	NoiseMat->setValue("u_persitance", persistance);
	NoiseMat->setValue("u_lacunarity", lacunarity);
	
	//pasing time to particles
	auto& particleUpdatePassMat = m_mainRenderer.getComputePass(ParticleComputeIdx + addDepthPasses).material;
	particleUpdatePassMat->setValue("timeStep", timestep);


}



void Lab2::onImGUIRender()
{



	float ms = 1000.0f / ImGui::GetIO().Framerate; ;  //milisecconds
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("GAMR3521");
	ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", ms, ImGui::GetIO().Framerate);  // display FPS and ms
	ImGui::ColorEdit3("Cube Colour", (float*)&m_colour);
	ImGui::Checkbox("wireframe", &m_wireFrame);
	if (ImGui::BeginTabBar("Tabs")) {
		if (ImGui::BeginTabItem("lighting")) {

			//colour changers
			ImGui::SliderFloat3("Fog Colour", glm::value_ptr(fogColour), 0.0f, 1.0f);
			ImGui::SliderFloat("fog density", &m_fogDensity, 0.0f, 0.5f);
			//displaying the shadow
			ImGui::SliderFloat3("light direction", glm::value_ptr(dl.direction), -1.0f, 1.0f);

			//tessalation level
			ImGui::SliderFloat("tes value 1", &value1, 10.f, 150.f);
			ImGui::SliderFloat("tes value 2", &value2, -0.5f, 0.5f);

			//chromatic aberration
			ImGui::SliderFloat("chrome strength", &expoStrength, 0.0f, 5.0f);


			//drawing a shadow map
			GLuint textureID = m_mainRenderer.getDepthPass(0).target->getTarget(0)->getID();
			ImVec2 imageSize = ImVec2(256, 256); //(width, height)
			ImVec2 uv0 = ImVec2(0.0f, 1.0f); //top left (flipped)
			ImVec2 uv1 = ImVec2(1.0f, 0.0f); //bottom right (flipped)
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();
			//gBuffer visulised
	
			textureID = m_mainRenderer.getRenderPass(1).target->getTarget(0)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

			textureID = m_mainRenderer.getRenderPass(1).target->getTarget(1)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);
			ImGui::SameLine();
	
			textureID = m_mainRenderer.getRenderPass(1).target->getTarget(2)->getID();
			ImGui::Image((void*)(intptr_t)textureID, imageSize, uv0, uv1);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("noise")) {

			ImGui::SliderInt("Octaves", &octaves,0,10);
			ImGui::SliderFloat("Amplitude", &amplitude, 0.0, 20.0);
			ImGui::SliderFloat("Frequency", &frequency, 0.0, 100.0);
			ImGui::SliderFloat("Persistance", &persistance, 0.0, 5.0);
			ImGui::SliderFloat("Lacunarity", &lacunarity, 0.0, 5.0);

			auto& computePass = m_mainRenderer.getComputePass(0);
			auto& computePass2 = m_mainRenderer.getComputePass(1);
			computePass.material->setValue("u_octaves", float(octaves));
			computePass.material->setValue("u_amplitude", amplitude);
			computePass.material->setValue("u_frequency", frequency);
			computePass.material->setValue("u_persitance", persistance);
			computePass.material->setValue("u_lacunarity", lacunarity);

			GLuint computeNoise = computePass.images[0].texture->getID();
			GLuint computeNormal = computePass2.images[0].texture->getID();
			ImVec2 imageSize = ImVec2(256, 256); //(width, height)
			ImVec2 uv0 = ImVec2(0.0f, 1.0f); //top left (flipped)
			ImVec2 uv1 = ImVec2(1.0f, 0.0f); //bottom right (flipped)
			ImGui::Image((void*)(intptr_t)computeNoise, imageSize, uv0, uv1);
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)computeNormal, imageSize, uv0, uv1);


			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("particles")) {

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::Render();
}

void Lab2::onKeyPressed(KeyPressedEvent& e)
{
	for (auto it = m_mainScene->m_actors.begin(); it != m_mainScene->m_actors.end(); ++it)
	{
		it->onKeyPress(e);
	}
}



