#include "MainMenuScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"

MainMenuScreen::MainMenuScreen()
{
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
	Entity& cameraEntity = Prefabs::createCamera(m_scene, { 0, 23 * 8, 26 }, { 0, 0, 5 }, { 0, 1, 0 });
	renderSystem->setCamera(&cameraEntity);
	
	// Create the main menu Start button
	createTextLabel("Start", glm::vec2(637.0f, 312.0f), &m_UIButtons);
	// Create the main menu controls button
	createTextLabel("Controls", glm::vec2(632.0f, 212.0f), &m_UIButtons);
	// Create the main menu quit button
	createTextLabel("Quit", glm::vec2(637.0f, 112.0f), &m_UIButtons);

	// Create the main menu title text
	createTextLabel("Battle Snake", glm::vec2(280.0f, 550.0f), &m_UITexts, 2.0f);
	// Create the main menu sub title text
	createTextLabel("Racers", glm::vec2(700.0f, 500.0f), &m_UITexts, 0.8f);
	// Create the main menu credits text
	createTextLabel("Made by: Lance Chaney, Jack Mair, Hugo Adams, Blair Corban", glm::vec2(400.0f, 10.0f), &m_UITexts, 0.5f);
	
	m_activeSystems.push_back(std::move(renderSystem));
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::update()
{
	for (auto& system : m_activeSystems) {
		system->beginFrame();
	}

	for (auto& system : m_activeSystems) {
		system->update();
	}

	for (size_t i = 0; i < m_UIButtons.size(); ++i)
	{
		m_UIButtons.at(i).Render();
	}

	for (size_t i = 0; i < m_UITexts.size(); ++i)
	{
		m_UITexts.at(i).Render();
	}

	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}