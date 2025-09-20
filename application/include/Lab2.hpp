#pragma once
#include <DemonRenderer.hpp>
#include "Grid.hpp"
using namespace std;

struct ShadowMapVars
{
	glm::vec3 centre;
	const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	float distanceAlongLightVector;
	float orthoSize;

	ShadowMapVars(glm::vec3 c = glm::vec3(0.0), float dist = 120.f) : centre(c), distanceAlongLightVector(dist) {
		orthoSize = distanceAlongLightVector * 1.2f;
	}
};


class Lab2 : public Layer
{
public:
	Lab2(GLFWWindowImpl& win);
private:
	void onRender() const override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onImGUIRender() override;

private:
	std::shared_ptr<Scene> m_mainScene;
	std::shared_ptr<Scene> m_screenScene;

	bool m_wireFrame{false};
	ShadowMapVars m_shadowMapVars;

	Renderer m_mainRenderer;
	Renderer m_initialseRenderer;
	// Actor positions for ease of use and to avoid additonal magic numbers
	size_t floorIdx;
	size_t particlesIdx;
	size_t modelIdx;
	size_t cameraIdx;
	size_t skyboxIdx;
	size_t bbIdx;
	//render pass indexes
	size_t depthIdx;
	size_t mainIdx;
	size_t LightIdx;
	size_t computeIdx;
	size_t textureComputeIdx;
	size_t ParticleComputeIdx;
	size_t fogIdx;
	size_t chromaticIdx;
	size_t visualDepthIdx;
	size_t screenIdx;
	//colours
	glm::vec3 fogColour = glm::vec3(1.0f,1.0f,1.0f);
	float m_fogDensity = 0.1f;
	float expoStrength = 2.0f;
	//tessalation values
	float value1 = 100.f;
	float value2 = -0.15f;
	//floor noise vars
	int octaves = 4;
	float amplitude = 7.0;
	float frequency = 14.0;
	float persistance = 0.9;
	float lacunarity = 3;
	
	DirectionalLight dl;
	glm::vec3 lightDir = glm::vec3(-0.2, -1.0, -0.5);
	glm::vec3 m_dlDirection;
	
	struct Vertex
	{
		glm::vec4 position{ glm::vec4(0.0) };
		glm::vec4 normal{ glm::vec4(0.0) };

	};
	struct Particle {
		glm::vec4 position;
		glm::vec4 velocity;
		float age;
	};

	int particleAmount = 100000;


	std::vector<float> skyboxVertices = {
		// positions          
		-100.f,  100.f, -100.f,
		-100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		 100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f, -100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f, -100.f,
		-100.f,  100.f,  100.f,
		-100.f, -100.f,  100.f,

		 100.f, -100.f, -100.f,
		 100.f, -100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f, -100.f,
		 100.f, -100.f, -100.f,

		-100.f, -100.f,  100.f,
		-100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		 100.f, -100.f,  100.f,
		-100.f, -100.f,  100.f,

		-100.f,  100.f, -100.f,
		 100.f,  100.f, -100.f,
		 100.f,  100.f,  100.f,
		 100.f,  100.f,  100.f,
		-100.f,  100.f,  100.f,
		-100.f,  100.f, -100.f,

		-100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f, -100.f,
		 100.f, -100.f, -100.f,
		-100.f, -100.f,  100.f,
		 100.f, -100.f,  100.f
	};

	std::vector<glm::vec3> vampirePos = {
		glm::vec3(-10, -5, -11),
		glm::vec3(-13, -5, -14),
		glm::vec3(-15, -5, -16),
		glm::vec3(-7, -5, -14),
		glm::vec3(-4, -5, -16)
	};
	//Gui
	glm::vec3 m_colour{ 1.0 };

};

