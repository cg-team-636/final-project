#pragma once

#include <vector>
using namespace std;


//	点
class Point {
public:
	float x;
	float y;
	float z;
	float texture_x;
	float texture_y;


	Point(float x = 0, float y = 0, float z = 0, float texture_x = 0, float texture_y = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->texture_x = texture_x;
		this->texture_y = texture_y;
	}
};


//	方块
class Block {
public:
	vector<Point> vertices;
	const float blockSize = 1.0f;

	//	给定方块中心点生成方块的顶点
	Block(Point center) {
		//	正面
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));

		//	后面
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 0));

		//	左面
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));


		//	右面
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));

		//	上面
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1));

		//	下面
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 1));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 1));
	}
};


float* blockToFloat(vector<Block*> blocks) {
	float* vertices = new float[blocks.size() * 36 * 5];
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < 36; j++) {
			vertices[(i * 36 + j) * 5 + 0] = blocks[i]->vertices[j].x;
			vertices[(i * 36 + j) * 5 + 1] = blocks[i]->vertices[j].y;
			vertices[(i * 36 + j) * 5 + 2] = blocks[i]->vertices[j].z;
			vertices[(i * 36 + j) * 5 + 3] = blocks[i]->vertices[j].texture_x;
			vertices[(i * 36 + j) * 5 + 4] = blocks[i]->vertices[j].texture_y;
		}
	}
	return vertices;
}



