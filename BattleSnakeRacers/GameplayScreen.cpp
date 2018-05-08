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
#include "OffTrackKillSystem.h"

#include <cmath>
#include <list>

TextLabel createPlayerTextLabel(int playerNum)
{
	std::string label = "Player " + toString(playerNum) + ": 05";
	TextLabel playerScore(label, "Assets/Fonts/NYCTALOPIATILT.TTF");
	playerScore.setPosition(glm::vec2(30.0f, 770.0f - ((playerNum - 1) * 30)));
	playerScore.setScale(0.4f);
	return playerScore;
}

GameplayScreen::GameplayScreen(std::array<bool, 4> activePlayers)
{
	// Screen Manager
	m_screenState = GAMEPLAY;
	// Init systems
	m_activeSystems.push_back(std::make_unique<InputSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<MovementSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<CollisionSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<PhysicsSystem>(m_scene));
	
	auto trackSystem = std::make_unique<TrackSystem>(m_scene);
	auto renderSystem = std::make_unique<RenderSystem>(m_scene);

	// Create environment map / skybox
	Entity& skybox = Prefabs::createSkybox(m_scene, GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays.dds"));
	Texture irradianceMap = GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays_iem.dds");
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
	Entity& player1 = Prefabs::createModel(m_scene, "Assets/Models/Plane/PUSHILIN_Plane.obj", playerTransform);
	player1.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
	player1.preTransform.eulerAngles.y = -glm::half_pi<float>();
	player1.transform.position.z = -25;
	player1.transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
	player1.sphereCollision.radius = 2;
	player1.inputMap.gamepadIdx = 0; // First gamepad plugged in
	player1.inputMap.turnAxisMap = 0; // Left stick x axis
	player1.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
	player1.inputMap.brakeBtnMap = 2;
	m_playerList.push_back(&player1);
	TextLabel player1Score = createPlayerTextLabel(1);
	player1Score.setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	m_playerScores.push_back(player1Score);
	//player1.playerStats.scoreLabel = &m_playerScores.back();

	// Setup player2
	if (activePlayers[1])
	{
		Entity& player2 = Prefabs::createModel(m_scene, "Assets/Models/UFO/PUSHILIN_flying_saucer.obj", playerTransform);
		player2.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
		player2.transform.position.z = -20;
		//player2.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		player2.sphereCollision.radius = 2;
		player2.inputMap.gamepadIdx = 1; // First gamepad plugged in
		player2.inputMap.turnAxisMap = 0; // Left stick x axis
		player2.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
		player2.inputMap.brakeBtnMap = 2;
		m_playerList.push_back(&player2);
		TextLabel player2Score = createPlayerTextLabel(2);
		player2Score.setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		m_playerScores.push_back(player2Score);
		//player2.playerStats.scoreLabel = &m_playerScores.back();

	}

	// Setup player3
	if (activePlayers[2])
	{
		Entity& player3 = Prefabs::createModel(m_scene, "Assets/Models/Police/PUSHILIN_Police_car.obj", playerTransform);
		player3.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
		player3.preTransform.eulerAngles.y = -glm::half_pi<float>();
		player3.transform.position.z = -15;
		player3.transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
		player3.sphereCollision.radius = 2;
		player3.inputMap.gamepadIdx = 2; // First gamepad plugged in
		player3.inputMap.turnAxisMap = 0; // Left stick x axis
		player3.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
		player3.inputMap.brakeBtnMap = 2;
		m_playerList.push_back(&player3);
		TextLabel player3Score = createPlayerTextLabel(3);
		player3Score.setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		//player3.playerStats.scoreLabel = &player3Score;
		m_playerScores.push_back(player3Score);
	}

	// Setup player4
	if (activePlayers[3])
	{
		Entity& player4 = Prefabs::createModel(m_scene, "Assets/Models/Taxi/PUSHILIN_Taxi.obj", playerTransform);
		player4.addComponents(COMPONENT_INPUT, COMPONENT_INPUT_MAP, COMPONENT_MOVEMENT, COMPONENT_PHYSICS, COMPONENT_PLAYERSTATS, COMPONENT_SPHERE_COLLISION);
		player4.preTransform.eulerAngles.y = -glm::half_pi<float>();
		player4.transform.position.z = -10;
		player4.transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
		player4.sphereCollision.radius = 2;
		player4.inputMap.gamepadIdx = 3; // First gamepad plugged in
		player4.inputMap.turnAxisMap = 0; // Left stick x axis
		player4.inputMap.accelerationBtnMap = 0; // A Button (Xbox controller)
		player4.inputMap.brakeBtnMap = 2;
		m_playerList.push_back(&player4);
		TextLabel player4Score = createPlayerTextLabel(4);
		player4Score.setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		//player4.playerStats.scoreLabel = &player4Score;
		m_playerScores.push_back(player4Score);
	}

	trackSystem->initializeTrackSystem();

	m_activeSystems.push_back(std::move(trackSystem));
	m_activeSystems.push_back(std::make_unique<OffTrackKillSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<PickupSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<CameraSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<SnakeTailSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<PlayerSpawnSystem>(m_scene, m_playerList));
	m_activeSystems.push_back(std::make_unique<CameraKillSystem>(m_scene, m_playerList, &cameraEntity));
	m_activeSystems.push_back(std::move(renderSystem));
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
		std::string kyle = "Player ";
		kyle += (char)(i + 1 + '0');
		kyle += ": ";
		kyle += static_cast<char>(m_playerList[i]->playerStats.currentScore + '0');
		m_playerScores[i].setText(kyle);
		m_playerScores.at(i).Render();
	}

	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}