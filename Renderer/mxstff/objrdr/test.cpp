#include "test.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
/*STRUCTURES AND CLASSES*/
#include "structures.hpp"
#include "face.hpp"

int main()
{
	std::string line;
	std::ifstream myFile("Models/Stafy/starfy.obj");
	/*std::ofstream out("output.txt");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
	*/

	mesh stafy;
	int numObjects = 0;

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			std::stringstream ss(line);
			if (line[0] != '#')
			{
				std::string term;
				while (ss >> term)
				{
					if (term == "mtllib")
					{
						ss >> stafy.materialLibrary;
					}
					else if (term == "o")
					{
						OBJObject newObject;
						stafy.objObjects.push_back(newObject);
						numObjects += 1;
						ss >> stafy.objObjects.at(numObjects - 1).name;
					}
					else if (term == "v")
					{
						vector3 newVert;
						ss >> newVert.x;
						ss >> newVert.y;
						ss >> newVert.z;
						stafy.objObjects.at(numObjects - 1).vertices.push_back(newVert);
					}
					else if (term == "vt")
					{
						vector2 newTC;
						ss >> newTC.x;
						ss >> newTC.y;
						stafy.objObjects.at(numObjects - 1).textureCoordinates.push_back(newTC);
					}
					else if (term == "usemtl")
					{
						ss >> stafy.objObjects.at(numObjects - 1).material;
					}
					else if (term == "s")
					{
						ss >> stafy.objObjects.at(numObjects - 1).smoothing;
					}
					else if (term == "f")
					{
						std::string part1;
						ss >> part1;
						std::string part2;
						ss >> part2;
						std::string part3;
						ss >> part3;

						auto makeFace = [&]()->face
						{
							auto makePieceList = [&](std::string part) -> std::vector<std::string>
							{
								std::string segment;
								std::stringstream evaluator(part);
								std::vector<std::string> list;
								while (std::getline(evaluator, segment, '/'))
								{
									list.push_back(segment);
								}
								return list;
							};

							std::vector<std::string> list1 = makePieceList(part1);
							std::vector<std::string> list2 = makePieceList(part2);
							std::vector<std::string> list3 = makePieceList(part3);

							switch (list1.size())
							{
							case 1:
								return face(
									stafy.objObjects[numObjects - 1].vertices[std::stof(list1.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list2.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list3.at(0))]);
								break;
							case 2:
								return face(
									stafy.objObjects[numObjects - 1].vertices[std::stof(list1.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list2.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list3.at(0))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list1.at(1))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list2.at(1))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list3.at(1))]);
								break;
							case 3:
								return face(
									stafy.objObjects[numObjects - 1].vertices[std::stof(list1.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list2.at(0))],
									stafy.objObjects[numObjects - 1].vertices[std::stof(list3.at(0))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list1.at(1))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list2.at(1))],
									stafy.objObjects[numObjects - 1].textureCoordinates[std::stof(list3.at(1))],
									stafy.objObjects[numObjects - 1].normals[std::stof(list1.at(2))],
									stafy.objObjects[numObjects - 1].normals[std::stof(list2.at(2))],
									stafy.objObjects[numObjects - 1].normals[std::stof(list3.at(2))]);
								break;
							default:
								std::cout << "ERROR: FACE DID NOT FIT KNOWN FORMAT" << std::endl;
								break;
							}
						};
						stafy.objObjects.at(numObjects - 1).faces.push_back(makeFace());
					}
				}
			}
		}
		myFile.close();
	}
	else
	{
		std::cout << "Unable to open file ...";
	}
};