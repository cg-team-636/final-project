#pragma once


#include "block.h"
#include <vector>
using namespace std;


//	创建 30 * 30的地板
vector<Block*> createFloor() {
	vector<Block*> floors;

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			Block* floor = new Block(Point(glm::vec3(j, 0, -i)), "textures/blocks/stonebrick.png");
			floors.push_back(floor);
		}
	}
	return floors;
}

vector<Block*> createTree() {
	vector<Block*> trees;

	for (int i = 0; i < 5; i++) {
		Block* treeBlock1 = new Block(Point(glm::vec3(7, 1 + i, -7)), "textures/blocks/log_oak.png");
		trees.push_back(treeBlock1);

		Block* treeBlock2 = new Block(Point(glm::vec3(21, 1 + i, -7)), "textures/blocks/log_oak.png");
		trees.push_back(treeBlock2);
	}

	return trees;
}

vector<Block*> createTreeLeaves() {
	vector<Block*> treeLeaves;

	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				Block* treeBlock1 = new Block(Point(glm::vec3(7 - 2 + i, 6 + z, -7 - 2 + j)), "textures/blocks/leaves_oak_green.png");
				treeLeaves.push_back(treeBlock1);

				Block* treeBlock2 = new Block(Point(glm::vec3(21 - 2 + i, 6 + z, -7 - 2 + j)), "textures/blocks/leaves_oak_green.png");
				treeLeaves.push_back(treeBlock2);
			}
		}
	}
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Block* treeBlock1 = new Block(Point(glm::vec3(7 - 1 + i, 8, -7 - 1 + j)), "textures/blocks/leaves_oak_green.png");
			treeLeaves.push_back(treeBlock1);

			Block* treeBlock2 = new Block(Point(glm::vec3(21 - 1 + i, 8, -7 - 1 + j)), "textures/blocks/leaves_oak_green.png");
			treeLeaves.push_back(treeBlock2);
		}
	}


	return treeLeaves;
}


vector<Block*> createGreenFloors() {
	vector<Block*> greenFloors;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Block* treeBlock1 = new Block(Point(glm::vec3(7 - 2 + i, 0, -7 - 2 + j)), "textures/blocks/wool_colored_lime.png");
			greenFloors.push_back(treeBlock1);

			Block* treeBlock2 = new Block(Point(glm::vec3(21 - 2 + i, 0, -7 - 2 + j)), "textures/blocks/wool_colored_lime.png");
			greenFloors.push_back(treeBlock2);
		}
	}

	return greenFloors;
}

vector<Block*> createDirtFloors() {
	vector<Block*> dirtFloors;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			Block* treeBlock1 = new Block(Point(glm::vec3(7 - 4 + i, 0, -7 - 4 + j)), "textures/blocks/dirt.png");
			dirtFloors.push_back(treeBlock1);

			Block* treeBlock2 = new Block(Point(glm::vec3(21 - 4 + i, 0, -7 - 4 + j)), "textures/blocks/dirt.png");
			dirtFloors.push_back(treeBlock2);
		}
	}

	return dirtFloors;
}

vector<Block*> createWaters() {
	vector<Block*> waters;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			Block* treeBlock1 = new Block(Point(glm::vec3(7 - 3 + i, 0, -7 - 3 + j)), "textures/blocks/wool_colored_blue.png");
			waters.push_back(treeBlock1);

			Block* treeBlock2 = new Block(Point(glm::vec3(21 - 3 + i, 0, -7 - 3 + j)), "textures/blocks/wool_colored_blue.png");
			waters.push_back(treeBlock2);
		}
	}

	return waters;
}

vector<Block*> createMainRoad() { 
	vector<Block*> mainRoad;

	return mainRoad;
}



