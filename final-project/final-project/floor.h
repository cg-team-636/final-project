#pragma once


#include "block.h"
#include <vector>
using namespace std;

//	�ذ干��20 * 20�ķ���
class Floor : public Block{
public:
	Floor(Point center) : Block(center) {}
};


//	���� 20 * 20�ĵذ�
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

