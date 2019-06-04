#pragma once


#include "block.h"
#include <vector>
using namespace std;

//	�ذ干��10 * 10�ķ���
class Floor : public Block{
public:
	Floor(Point center) : Block(center) {}
};


//	���� 10 * 10�ĵذ�
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

