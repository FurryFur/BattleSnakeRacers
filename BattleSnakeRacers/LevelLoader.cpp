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



const int MaxMapWidth = 40;
const int MaxMapHeight = 40; // must be even

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

void TranslateCharLevel(const NDArray<char, MaxMapWidth, MaxMapHeight> charLevel, Scene& scene)
{
	//TransformComponent pickupTransform{};
	//pickupTransform.scale.x = 0.5f;
	//pickupTransform.scale.y = 0.5f;
	//pickupTransform.scale.z = 0.5f;

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
			else // (charLevel[i][j] == '1')
			{
				bool isStraight = false;
				float rotation = 0;
				switch (charLevel[i][j])
				{
				case 'u':
				{
					isStraight = true;
					break;
				}
				case 'l':
				{
					rotation = 90;
					isStraight = true;
					break;
				}
				case 'd':
				{
					rotation = 180;
					isStraight = true;
					break;
				}
				case 'r':
				{
					rotation = 270;
					isStraight = true;
					break;
				}
				case '1':
				{
					
					isStraight = false;
					break;
				}
				case '2':
				{
					rotation = 270;
					isStraight = false;
					break;
				}
				case '3':
				{
					rotation = 180;
					isStraight = false;
					break;
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

				
				Entity& en = scene.createEntity(COMPONENT_MODEL | COMPONENT_TRANSFORM);
				if (isStraight)
				{	//straightline
					en.model = ModelUtils::loadModel("Assets/Models/Track/straightUp.obj");
				}
				else
				{
					//curved piece
					en.model = ModelUtils::loadModel("Assets/Models/Track/curve1.obj");
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

				//Entity& pickup = Prefabs::createSphere(scene, pickupTransform);
				//pickup.transform.position = glm::vec3(-25 + j * 2, 0.5f, i * 2);
				//pickup.addComponents(COMPONENT_PICKUP);
			}
		}
	}


}

void CreateLevel(Scene& scene, std::string path)
{
	TranslateCharLevel(ReadFile(path), scene);
}
