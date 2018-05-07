#define _USE_MATH_DEFINES
#include "GameplayScreen.h"

#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "PickupSystem.h"
#include "MovementSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "SnakeTailSystem.h"
#include "PlayerSpawnSystem.h"
#include "Utils.h"
#include "CollisionSystem.h"
#include "TrackSystem.h"
#include "CameraKillSystem.h"

#include <cmath>
#include <list>

GameplayScreen::GameplayScreen()
{
	// Screen Manager
	m_screenState = GAMEPLAY;
	// Init systems
	m_activeSystems.push_back(std::make_unique<InputSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<MovementSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<CollisionSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<PhysicsSystem>(m_scene));
	
	//auto trackSystem = std::make_unique<TrackSystem>(m_scene);
	auto renderSystem = std::make_unique<RenderSystem>(m_scene);

	// Create environment map / skybox
	Entity& skybox = Prefabs::createSkybox(m_scene, {
		"Assets/Textures/envmap_violentdays/violentdays_rt.tga",
		"Assets/Textures/envmap_violentdays/violentdays_lf.tga",
		"Assets/Textures/envmap_violentdays/violentdays_up.tga",
		"Assets/Textures/envmap_violentdays/violentdays_dn.tga",
		"Assets/Textures/envmap_violentdays/violentdays_bk.tga",
		"Assets/Textures/envmap_violentdays/violentdays_ft.tga",
	});
	Texture irradianceMap = GLUtils::loadCubeMapFaces({
		"Assets/Textures/envmap_violentdays/violentdays_irr_c00.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c01.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c02.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c03.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c04.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c05.bmp",
	});
	Texture radianceMap = GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays_pmrem.dds");
	renderSystem->setRadianceMap(radianceMap.id);
	renderSystem->setIrradianceMap(irradianceMap.id);

	// Setup the camera
	Entity& cameraEntity = Prefabs::createCamera(m_scene, { 0, 23*8, 26 }, { 0, 0, 5 }, { 0, 1, 0 });
	renderSystem->setCamera(&cameraEntity);

	// Create Ground
	/*TransformComponent groundTransform{};
	groundTransform.eulerAngles.x = M_PI / 2.0f;
	groundTransform.position.y = -1;
	groundTransform.scale *= 100;
	Prefabs::createQuad(m_scene, groundTransform);*/
	CreateLevel(m_scene,"Assets/Maps/Level2.txt");
	
	TransformComponent pickupTransform{};
	pickupTransform.scale.x = 3;
	pickupTransform.scale.y = 3;
	pickupTransform.scale.z = 3;
	Entity& pickup1 = Prefabs::createModel(m_scene, "Assets/Models/crystal/mese.obj", pickupTransform);
	pickup1.transform.position = glm::vec3(-20, 1, -20);
	pickup1.addComponents(COMPONENT_PICKUP);
	pickup1.pickup.isActive = true;

	
	// Setup player1
	TransformComponent playerTransform{};
	//playerTransform.scale.x = 1.0f;
	playerTransform.eulerAngles.y += 1.5708f;
	Entity& player1 = Prefabs::createModel(m_scene, "Assets/Models/truck/truck1.FBX", playerTransform);
	player1.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
	player1.transform.position.z = -15;
	player1.sphereCollision.radius = 2;
	player1.inputMap.gamepadIdx = 0; // First gamepad plugged in
	player1.inputMap.turnAxisMap = 0; // Left stick x axis
	player1.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
	player1.inputMap.brakeBtnMap = 2;

	// Setup player2
	Entity& player2 = Prefabs::createModel(m_scene, "Assets/Models/truck/truck1.FBX", playerTransform);
	player2.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
	player2.transform.position.z = -10;
	player2.sphereCollision.radius = 2;
	player2.inputMap.gamepadIdx = 1; // First gamepad plugged in
	player2.inputMap.turnAxisMap = 0; // Left stick x axis
	player2.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
	player2.inputMap.brakeBtnMap = 2;

	// Setup player3
	Entity& player3 = Prefabs::createModel(m_scene, "Assets/Models/truck/truck1.FBX", playerTransform);
	player3.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
	player3.transform.position.z = -5;
	player3.sphereCollision.radius = 2;
	player3.inputMap.gamepadIdx = 2; // First gamepad plugged in
	player3.inputMap.turnAxisMap = 0; // Left stick x axis
	player3.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
	player3.inputMap.brakeBtnMap = 2;

	// Setup player4
	Entity& player4 = Prefabs::createModel(m_scene, "Assets/Models/truck/truck1.FBX", playerTransform);
	player4.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
	player4.transform.position.z = 0;
	player4.sphereCollision.radius = 2;
	player4.inputMap.gamepadIdx = 3; // First gamepad plugged in
	player4.inputMap.turnAxisMap = 0; // Left stick x axis
	player4.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
	player4.inputMap.brakeBtnMap = 2;
	
	m_playerList.push_back (&player1);
	m_playerList.push_back (&player2);
	m_playerList.push_back (&player3);
	m_playerList.push_back (&player4);

	//trackSystem->initializeTrackSystem();

	//m_activeSystems.push_back(std::move(trackSystem));
	m_activeSystems.push_back(std::make_unique<PickupSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<CameraSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<SnakeTailSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<PlayerSpawnSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<CameraKillSystem>(m_scene, m_playerList, &cameraEntity));
	m_activeSystems.push_back(std::move(renderSystem));

	// Create text labels for each players score
	for (size_t i = 0; i < m_playerList.size(); ++i)
	{
		std::string label = "Player " + toString(i + 1) + ": 05";
		TextLabel playerScore(label, "Assets/Fonts/NYCTALOPIATILT.TTF");
		playerScore.setPosition(glm::vec2(30.0f, 770.0f - (i * 30)));
		playerScore.setScale(0.4f);

		if (i == 0)
		{
			playerScore.setColor(glm::vec3(1.0f, 0.5f, 0.8f));
			player1.playerStats.scoreLabel = &playerScore;
		}
		else if (i == 1)
		{
			playerScore.setColor(glm::vec3(0.5f, 0.8f, 1.0f));
			player2.playerStats.scoreLabel = &playerScore;
		}
		else if (i == 2)
		{
			playerScore.setColor(glm::vec3(1.0f, 1.0f, 0.2f));
			player3.playerStats.scoreLabel = &playerScore;
		}
		else
		{
			playerScore.setColor(glm::vec3(0.5f, 1.0f, 0.8f));
			player4.playerStats.scoreLabel = &playerScore;
		}

		m_playerScores.push_back(playerScore);
	}
}

GameplayScreen::~GameplayScreen()
{
}

void GameplayScreen::update()
{
	for (auto& system : m_activeSystems) {
		system->beginFrame();
	}

	for (auto& system : m_activeSystems) {
		system->update();
	}

	for (size_t i = 0; i < m_playerScores.size(); ++i)
	{
		m_playerScores.at(i).Render();
	}

	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}