#define _USE_MATH_DEFINES

#include "EngineEntityDefs.h"

#include "GLUtils.h"
#include "GLPrimitives.h"
#include "Scene.h"
#include "Entity.h"
#include "ModelUtils.h"

#include <glm\gtc\matrix_transform.hpp>

#include "Log.h"
#include <memory>

namespace Game {

	Entity& createQuad(Scene& scene, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.transform = transform;


		entity.model = GLPrimitives::getQuadModel();
		entity.model.materials.at(0).shaderParams.glossiness = 0.0f;
		entity.model.materials.at(0).shaderParams.metallicness = 0.0f;
		entity.model.materials.at(0).shaderParams.specBias = 0;

		setDefaultInputBindings(entity.inputMap);

		// Replace default texture
		entity.model.materials.at(0).colorMaps.at(0) = GLUtils::loadTexture("Assets/Textures/dessert-floor.png");

		return entity;
	}

	Entity& createSphere(Scene& scene, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.transform = transform;

		entity.model = GLPrimitives::getSphereModel();

		setDefaultInputBindings(entity.inputMap);

		return entity;
	}

	Entity& createCylinder(Scene& scene, float radius, float height, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.transform = transform;
		entity.transform.scale.x *= radius;
		entity.transform.scale.y *= height;
		entity.transform.scale.z *= radius;

		entity.model = GLPrimitives::getCylinderModel();

		setDefaultInputBindings(entity.inputMap);

		return entity;
	}

	Entity& createPyramid(Scene& scene, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.transform = transform;

		entity.model = GLPrimitives::getPyramidModel();

		setDefaultInputBindings(entity.inputMap);

		return entity;
	}

	Entity& createCube(Scene& scene, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.transform = transform;

		entity.model = GLPrimitives::getCubeModel();

		setDefaultInputBindings(entity.inputMap);

		return entity;
	}

	Entity& createCamera(Scene& scene, const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
	{
		Entity& entity = scene.createEntity(COMPONENT_CAMERA);

		entity.camera.lookAt = glm::lookAt(pos, center, up);
		entity.camera.position = pos;

		return entity;
	}

	Entity& createSkybox(Scene& scene, const std::vector<std::string>& faceFilenames)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL);

		entity.model = GLPrimitives::getCubeModel();

		// Replace default material
		entity.model.materials.at(0) = {};
		entity.model.materials.at(0).shader = GLUtils::getSkyboxShader();
		entity.model.materials.at(0).colorMaps.push_back(GLUtils::loadCubeMapFaces(faceFilenames));
		entity.model.materials.at(0).willDrawDepth = false;

		return entity;
	}

	Entity& createModel(Scene& scene, const std::string& path, const TransformComponent& transform)
	{
		Entity& entity = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);

		entity.model = ModelUtils::loadModel(path);

		entity.transform = transform;

		return entity;
	}

	void setDefaultInputBindings(InputMapComponent& inputMap)
	{
		inputMap = {};
		inputMap.leftBtnMap = GLFW_KEY_KP_4;
		inputMap.rightBtnMap = GLFW_KEY_KP_6;
		inputMap.forwardBtnMap = GLFW_KEY_KP_8;
		inputMap.backwardBtnMap = GLFW_KEY_KP_5;
		inputMap.downBtnMap = GLFW_KEY_KP_7;
		inputMap.upBtnMap = GLFW_KEY_KP_9;
		inputMap.azimuthPosBtnMap = GLFW_KEY_KP_1;
		inputMap.azimuthNegBtnMap = GLFW_KEY_KP_2;
		inputMap.elevationPosBtnMap = GLFW_KEY_KP_3;
		inputMap.elevationNegBtnMap = GLFW_KEY_KP_DECIMAL;
		inputMap.rollBtnMap = GLFW_KEY_KP_0;
		inputMap.btn1Map = GLFW_KEY_KP_ADD;
		inputMap.btn2Map = GLFW_KEY_KP_SUBTRACT;
		inputMap.btn3Map = GLFW_KEY_KP_MULTIPLY;
		inputMap.btn4Map = GLFW_KEY_KP_DIVIDE;
	}
}