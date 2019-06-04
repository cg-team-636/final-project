#pragma once


#include "block.h"
#include <vector>
using namespace std;

//	地板共有10 * 10的方块
class Floor : public Block{
public:
	Floor(Point center) : Block(center) {}
};


//	创建 10 * 10的地板
vector<Floor*> createFloor() {
	vector<Floor*> floors;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Floor* floor = new Floor(Point(j, 0, -i));
			floors.push_back(floor);
		}
	}
	return floors;
}

