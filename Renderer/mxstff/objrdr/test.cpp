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
	//std::ofstream out("output.txt");
	//std::streambuf *coutbuf = std::cout.rdbuf();
	//std::cout.rdbuf(out.rdbuf());

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
						//std::cout << stafy.materialLibrary << std::endl;
					}
					else if (term == "o")
					{
						OBJObject newObject;
						stafy.objObjects.push_back(newObject);
						numObjects += 1;
						ss >> stafy.objObjects.at(numObjects - 1).name;
						//std::cout << stafy.objObjects.at(numObjects - 1).name << std::endl;
					}
					else if (term == "v")
					{
						vector3 newVert;
						ss >> newVert.x;
						ss >> newVert.y;
						ss >> newVert.z;
						stafy.objObjects.at(numObjects - 1).vertices.push_back(newVert);
						//std::cout << stafy.objObjects.at(numObjects - 1).vertices[stafy.objObjects.at(numObjects - 1).vertices.size() - 1].x << std::endl;
						//std::cout << stafy.objObjects.at(numObjects - 1).vertices[stafy.objObjects.at(numObjects - 1).vertices.size() - 1].y << std::endl;
						//std::cout << stafy.objObjects.at(numObjects - 1).vertices[stafy.objObjects.at(numObjects - 1).vertices.size() - 1].z << std::endl;
					}
					else if (term == "vt")
					{
						vector2 newTC;
						ss >> newTC.x;
						ss >> newTC.y;
						stafy.objObjects.at(numObjects - 1).textureCoordinates.push_back(newTC);
						//std::cout << stafy.objObjects.at(numObjects - 1).textureCoordinates[stafy.objObjects.at(numObjects - 1).textureCoordinates.size() - 1].x << std::endl;
						//std::cout << stafy.objObjects.at(numObjects - 1).textureCoordinates[stafy.objObjects.at(numObjects - 1).textureCoordinates.size() - 1].y << std::endl;
					}
					else if (term == "usemtl")
					{
						ss >> stafy.objObjects.at(numObjects - 1).material;
						//std::cout << stafy.objObjects.at(numObjects - 1).material << std::endl;
					}
					else if (term == "s")
					{
						ss >> stafy.objObjects.at(numObjects - 1).smoothing;
						//std::cout << stafy.objObjects.at(numObjects - 1).smoothing << std::endl;
					}
					else if (term == "f")
					{
						std::string part1;
						ss >> part1;
						std::string part2;
						ss >> part2;
						std::string part3;
						ss >> part3;
						//std::cout << part1 << std::endl;
						//std::cout << part2 << std::endl;
						//std::cout << part3 << std::endl;

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
								//for (auto l : list){std::cout << l << std::endl;}
								return list;
							};

							std::vector<std::string> list1 = makePieceList(part1);
							std::vector<std::string> list2 = makePieceList(part2);
							std::vector<std::string> list3 = makePieceList(part3);
							//for (auto l : list1){std::cout << l << std::endl;}
							//for (auto l : list2){std::cout << l << std::endl;}
							//for (auto l : list3){std::cout << l << std::endl;}

							switch (list1.size())
							{
							case 1:
								return face(
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list1.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list2.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list3.at(0)) - 1)]);
								break;
							case 2:
								return face(
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list1.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list2.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list3.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list1.at(1)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list2.at(1)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list3.at(1)) - 1)]);
								break;
							case 3:
								return face(
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list1.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list2.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).vertices[static_cast<int>(std::stof(list3.at(0)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list1.at(1)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list2.at(1)) - 1)],
									stafy.objObjects.at(numObjects - 1).textureCoordinates[static_cast<int>(std::stof(list3.at(1)) - 1)],
									stafy.objObjects.at(numObjects - 1).normals[static_cast<int>(std::stof(list1.at(2)) - 1)],
									stafy.objObjects.at(numObjects - 1).normals[static_cast<int>(std::stof(list2.at(2)) - 1)],
									stafy.objObjects.at(numObjects - 1).normals[static_cast<int>(std::stof(list3.at(2)) - 1)]);
								break;
							default:
								std::cout << "ERROR: FACE DID NOT FIT KNOWN FORMAT" << std::endl;
								break;
							}
							return face();
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