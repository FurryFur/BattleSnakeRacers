//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : In Progress
// Author       : Hugo Adams
// Mail         : Hugo.ada7271@mediadesign.school.nz
//

#include "LevelLoader.h"
#include "Utils.h"
#include "Entity.h"
#include "Scene.h"
#include "PrimitivePrefabs.h"
#include "ModelUtils.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <glm\gtc\matrix_transform.hpp>


const int MaxMapWidth = 40;
const int MaxMapHeight = 40; // must be even

std::string GetFilePath(std::string _file);

NDArray<char,MaxMapWidth,MaxMapHeight> ReadFile(std::string _path)
{
	NDArray<char, MaxMapWidth, MaxMapHeight> charLevel;

	for (int i = 0; i < MaxMapHeight; i++)  // Setting all of charLevel to ignore chara 
	{
		for (int j = 0; j < MaxMapWidth; j++)
		{
			charLevel[i][j] = '0';
		}
	}

	std::fstream myfile;
	myfile.open(_path);
	std::string line;

	for (int i = 0; i < MaxMapHeight; i++)
	{
		if (std::getline(myfile, line))
		{
			for (int j = 0; j < line.size(); j++)
			{
				charLevel[i][j] = line[j];
			}
		}
	}

	myfile.close();

	return charLevel;
}

NDArray<char, 20, 20> ItemFile(std::string _path)
{
	NDArray<char, 20, 20> itemPlace;
	for (int i = 0; i < 20; i++)  // Setting all of charLevel to ignore chara 
	{
		for (int j = 0; j < 20; j++)
		{
			itemPlace[i][j] = 'x';
		}
	}

	std::fstream myfile;
	myfile.open(_path);
	std::string line;

	for (int i = 0; i < 20; i++)
	{
		if (std::getline(myfile, line))
		{
			for (int j = 0; j < line.size(); j++)
			{
				itemPlace[i][j] = line[j];
			}
		}
	}

	myfile.close();

	return itemPlace;
}


glm::vec3 TranslateCharLevel(std::string _path, Scene& scene)
{
	glm::vec3 retval;
	NDArray<char, 20, 20> straightTrack = ItemFile("Assets/Maps/straight road item.txt");
	NDArray<char, 20, 20> curveTrack = ItemFile("Assets/Maps/boxcorner road item.txt");

	std::string b = GetFilePath(_path);
	std::string  cfileNum = b.substr(b.size() - 1, 1);

	std::string str8file = "Assets/Models/Track/straight/straight" + cfileNum;
	str8file += "/straightUp.obj";
	std::string curvefile = "Assets/Models/Track/boxEdge/boxEdge" + cfileNum;
	curvefile += "/boxEdge.obj";

	std::string progression = GetFilePath(_path) + "Progression.txt";
	std::string respawn = GetFilePath(_path) + "Respawn.txt";
	NDArray<char, MaxMapWidth, MaxMapHeight> LevelProg, LevelRes, charLevel;
	charLevel = ReadFile(_path);
	LevelProg= ReadFile(progression);
	LevelRes = ReadFile(respawn);
	// make object vector here 
	float fscale = 1.0f;
	

	for (int i = 0; i < MaxMapHeight; i++)
	{
		for (int j = 0; j < MaxMapWidth; j += 1)
		{
			if (charLevel[i][j] == '0')
			{
				continue;
			}
			else 
			{
				bool isStraight = false;
				float rotation = 0;
				bool spawnPickUps = false;

				switch (charLevel[i][j])
				{
				case 'U':
				{
					spawnPickUps = true;
				}
				case 'u':
				{
					isStraight = true;
					break;
				}
				case 'L':
				{
					spawnPickUps = true;
				}
				case 'l':
				{
					rotation = 90;
					isStraight = true;
					break;
				}
				case 'D':
				{
					spawnPickUps = true;
				}
				case 'd':
				{
					rotation = 180;
					isStraight = true;
					break;
				}
				case 'R':
				{
					spawnPickUps = true;
				}
				case 'r':
				{
					rotation = 270;
					isStraight = true;
					break;
				}
				case '5':
				{
					spawnPickUps = true;
				}
				case '1':
				{
					isStraight = false;
					break;
				}
				case '6':
				{
					spawnPickUps = true;
				}
				case '2':
				{
					rotation = 270;
					isStraight = false;
					break;
				}
				case '7':
				{
					spawnPickUps = true;
				}
				case '3':
				{
					rotation = 180;
					isStraight = false;
					break;
				}
				case '8':
				{
					spawnPickUps = true;
				}
				case '4':
				{
					rotation = 90;
					isStraight = false;
					break;
				}
				default:
				{
					continue;
				}
				}

				
				Entity& en = scene.createEntity(COMPONENT_MODEL, COMPONENT_TRANSFORM);
				NDArray<char, 20, 20> track;
				if (isStraight)
				{	//straightline
					en.model = ModelUtils::loadModel(str8file);
					track = straightTrack;
				}
				else
				{
					//curved piece
					en.model = ModelUtils::loadModel(curvefile);
					track = curveTrack;
				}
				
				// make object here and put in an vector
				//set its position and scale
				
				while (rotation >= 360)
				{
					rotation -= 360;
				}
				
				en.transform.position = glm::vec3(j * (fscale * 50.0f), -10 * fscale, (i * (fscale * 50.0f)));
				en.transform.eulerAngles = glm::vec3(270 * 3.14159/180, rotation * 3.14159/180, 0);
				en.transform.scale = glm::vec3(fscale, fscale, fscale);
				
				char cdebug = charLevel[i][j];

				for (int xoff = 0; xoff < 2; xoff++)
				{
					for (int yoff = 0; yoff < 2; yoff++)
					{
						char cLPrg = LevelProg[i + xoff][j + yoff];
						if (cLPrg != '0')
						{
							//Entity& enb = Prefabs::createSphere(scene);
							//enb.addComponents(COMPONENT_TRANSFORM, COMPONENT_LEVELPROG);

							Entity& enb = scene.createEntity(COMPONENT_LEVELPROG | COMPONENT_TRANSFORM);
							enb.levelProg.priority = (unsigned int)cLPrg;
							enb.transform.position = en.transform.position;
							enb.transform.position.x += ((fscale * -25.0f) + (yoff * 50.0f * fscale));
							enb.transform.position.z += ((fscale * -25.0f) + (xoff * 50.0f * fscale));
							enb.transform.position.y = 0;
							if (cLPrg == 'A')
							{
								retval = enb.transform.position;
							}
							if (cLPrg != LevelRes[i + xoff][j + yoff])
							{
								enb.levelProg.isRespawn = true;
								enb.levelProg.respawnDirection = LevelRes[i + xoff][j + yoff];
							}
						}
					}
				}

				if (spawnPickUps == true)
				{
					for (int pui = 0; pui < 19; pui++)
					{
						for (int puj = 0; puj < 19; puj++)
						{
							if (track[pui][puj] != 'x' && track[pui][puj] != ' ')
							{
								if (track[pui][puj] != '9')
								{
									continue;
								}

								TransformComponent pickupTransform{};
								pickupTransform.scale = glm::vec3(3, 3, 3);
								Entity& pickup1 = Prefabs::createModel(scene, "Assets/Models/crystal/mese.obj", pickupTransform);
								//Entity& pickup = Prefabs::createSphere(scene);
								//pickup1.transform.position = glm::vec3(-20, 1, -20);
								//pickup1.addComponents(COMPONENT_PICKUP);
								pickup1.pickup.isActive = true;

								pickup1.transform.position.x = (-45.0f + 10.0f * pui);
								pickup1.transform.position.z = (-45.0f + 10.0f * puj * 0.5f);

								if (isStraight)
								{
									glm::mat4 rot = glm::rotate(glm::mat4(), (rotation - 90) * 3.14159f / 180.0f, glm::vec3(0, 1, 0));
									pickup1.transform.position = rot * glm::vec4(pickup1.transform.position, 1.0f);
								}
								else
								{
									glm::mat4 rot = glm::rotate(glm::mat4(), (rotation - 180) * 3.14159f / 180.0f, glm::vec3(0, 1, 0));
									pickup1.transform.position = rot * glm::vec4(pickup1.transform.position, 1.0f);
								}

								pickup1.transform.position += en.transform.position;
								pickup1.transform.position.y = 1;
								pickup1.addComponents(COMPONENT_PICKUP);
							}
						}
					}
				}
			}
		}
	}
	return retval;
}

glm::vec3 CreateLevel(Scene& scene, std::string path)
{
	return TranslateCharLevel(path, scene);
}

std::string GetFilePath(std::string _file)
{
	std::string prog;
	for (int i = 0; i < _file.length(); i++)
	{
		if (_file.at(i) == '.')
		{
			prog = _file.substr(0, i);
			break;
		}
	}
	return prog;
}