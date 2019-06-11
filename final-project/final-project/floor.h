#pragma once


#include "block.h"
#include <vector>
using namespace std;


//	创建 20 * 20的地板
vector<Block*> createFloor() {
	vector<Block*> floors;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Block* floor = new Block(Point(glm::vec3(j, 0, -i)), "textures/blocks/dirt.png");
			floors.push_back(floor);
		}
	}
	return floors;
}

