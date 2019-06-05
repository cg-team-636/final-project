#pragma once


#include "block.h"
#include <vector>
using namespace std;

//	地板共有20 * 20的方块
class Floor : public Block{
public:
	Floor(Point center) : Block(center) {}
};


//	创建 20 * 20的地板
vector<Floor*> createFloor() {
	vector<Floor*> floors;

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			Floor* floor = new Floor(Point(glm::vec3(j, 0, -i)));
			floors.push_back(floor);
		}
	}
	return floors;
}

