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

#include <fstream>
#include <iostream>
#include <vector>
#include <string>


const int MaxMapWidth = 40;
const int MaxMapHeight = 40; // must be even

NDArray<char,MaxMapWidth,MaxMapHeight> ReadFile()
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
	myfile.open("Assets/Maps/Level0.txt");
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
	// make object vector here 
	for (int i = 0; i < MaxMapHeight; i++)
	{
		for (int j = 0; j < MaxMapWidth; j += 1)
		{
			if (charLevel[i][j] == '0')
			{
				continue;
			}
			else if (charLevel[i][j] == '1')
			{
				// make object here and put in an vector
				//set its position and scale
				Entity& en = Prefabs::createCube(scene);
				//scene.createEntity(COMPONENT_MODEL, COMPONENT_TRANSFORM);
				en.transform.position = glm::vec3(-25 +j * 2, 0, i * 2);
			}
			
		}
	}


}

void CreateLevel(Scene& scene)
{
	TranslateCharLevel(ReadFile(), scene);
}
