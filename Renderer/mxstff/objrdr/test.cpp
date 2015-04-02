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

/*FUNCTION FORWARDS*/
void printMesh(struct mesh& myMesh);

int main()
{
	std::string line;
	std::ifstream myFile("Models/Stafy/starfy.obj");
	std::ofstream out("output.txt");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());

	mesh stafy;
	int numObjects = 0;
	int numMaterials = 0;

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			std::stringstream ss(line);
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
				else if (term == "vn")
				{
					vector3 newNorm;
					ss >> newNorm.x;
					ss >> newNorm.y;
					ss >> newNorm.z;
					stafy.objObjects.at(numObjects - 1).normals.push_back(newNorm);
					//std::cout << stafy.objObjects.at(numObjects - 1).normals[stafy.objObjects.at(numObjects - 1).normals.size() - 1].x << std::endl;
					//std::cout << stafy.objObjects.at(numObjects - 1).normals[stafy.objObjects.at(numObjects - 1).normals.size() - 1].y << std::endl;
					//std::cout << stafy.objObjects.at(numObjects - 1).normals[stafy.objObjects.at(numObjects - 1).normals.size() - 1].z << std::endl;
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
		myFile.close();
		std::ifstream myFile("Models/Stafy/" + stafy.materialLibrary);
		if (myFile.is_open())
		{
			while (getline(myFile, line))
			{
				std::stringstream ss(line);
				std::string term;
				while (ss >> term)
				{
					if (term == "newmtl")
					{
						material newMat;
						stafy.objMaterials.materialList.push_back(newMat);
						numMaterials += 1;
						ss >> stafy.objMaterials.materialList.at(numMaterials - 1).name;
					}
					else if (term == "Ka")
					{
						vector3 newAmb;
						ss >> newAmb.x;
						ss >> newAmb.y;
						ss >> newAmb.z;
						stafy.objMaterials.materialList.at(numMaterials - 1).ambient = newAmb;
					}
					else if (term == "Kd")
					{
						vector3 newDif;
						ss >> newDif.x;
						ss >> newDif.y;
						ss >> newDif.z;
						stafy.objMaterials.materialList.at(numMaterials - 1).diffuse = newDif;
					}
					else if (term == "Ks")
					{
						vector3 newSpec;
						ss >> newSpec.x;
						ss >> newSpec.y;
						ss >> newSpec.z;
						stafy.objMaterials.materialList.at(numMaterials - 1).specular = newSpec;
					}
					else if (term == "illum")
					{
						ss >> stafy.objMaterials.materialList.at(numMaterials - 1).illumination;
					}
					else if (term == "Ns")
					{
						ss >> stafy.objMaterials.materialList.at(numMaterials - 1).shininess;
					}
					else if (term == "d" || term == "Tr")
					{
						ss >> stafy.objMaterials.materialList.at(numMaterials - 1).transparencey;
					}
					else if (term == "map_Ka" || term == "map_Kd")
					{
						ss >> stafy.objMaterials.materialList.at(numMaterials - 1).textureName;
					}
				}
			}
		}
		myFile.close();
		for (auto m : stafy.objMaterials.materialList)
		{
			std::ifstream myFile("Models/Stafy/" + m.textureName, std::ios::in | std::ios::binary);
			if (myFile.is_open())
			{
				std::ostringstream ss;
				ss << myFile.rdbuf();
				stafy.objMaterials.textures.push_back(ss.str());
			}
		}
		myFile.close();
		printMesh(stafy);
	}
	else
	{
		std::cout << "Unable to open file ...";
	}
};

void printMesh(struct mesh& myMesh)
{
	std::cout << myMesh.materialLibrary << std::endl;
	for (auto o : myMesh.objObjects)
	{
		std::cout << "OBJECT NAME = " << o.name << std::endl;
		std::cout << "OBJECT MATERIAL = " << o.material << std::endl;
		std::cout << "VERTICES" << std::endl;
		for (auto v : o.vertices)
		{
			std::cout << v.x << " " << v.y << " " << v.z << std::endl;
		}
		std::cout << "TEXTURE COORDINATES" << std::endl;
		for (auto t : o.textureCoordinates)
		{
			std::cout << t.x << " " << t.y << std::endl;
		}
		std::cout << "VERTEXT NORMALS" << std::endl;
		for (auto n : o.normals)
		{
			std::cout << n.x << " " << n.y << " " << n.z << std::endl;
		}
		std::cout << "FACES" << std::endl;
		for (auto f : o.faces)
		{
			std::cout << f.posX << " " << f.posY << " " << f.posZ << std::endl;
			std::cout << f.tcX << " " << f.tcY << " " << f.tcZ << std::endl;
			std::cout << f.normX << " " << f.normY << " " << f.normZ << std::endl;
		}
		std::cout << "SMOOTHING = " << o.smoothing<< std::endl;
	}
	std::cout << "OBJ MATERIAL LIST" << std::endl;
	for (auto m : myMesh.objMaterials.materialList)
	{
		std::cout << "MATERIAL NAME = " << m.name << std::endl;
		std::cout << "AMBIENT = " << m.ambient.x << " " << m.ambient.y << " " << m.ambient.z << std::endl;
		std::cout << "DIFFUSE = " << m.diffuse.x << " " << m.diffuse.y << " " << m.diffuse.z << std::endl;
		std::cout << "SPECULAR = " << m.specular.x << " " << m.specular.y << " " << m.specular.z << std::endl;
		std::cout << "ILLUMINATION = " << m.illumination << std::endl;
		std::cout << "SHININESS = " << m.shininess << std::endl;
		std::cout << "TRANSPARENCY = " << m.transparencey << std::endl;
		std::cout << "TEXTURE FILE = " << m.textureName << std::endl;
	}
	std::cout << "OBJ TEXTURE LIST" << std::endl;
	for (auto t : myMesh.objMaterials.textures)
	{
		std::cout << "TEXTURE" << std::endl;
		std::cout << t << std::endl;
	}
};