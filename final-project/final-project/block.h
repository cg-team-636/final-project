#pragma once

#include <vector>
using namespace std;


//	��
class Point {
public:
	//	�����ά����
	float x;
	float y;
	float z;

	//	��Ĳ�������
	float texture_x;
	float texture_y;

	//	��ķ���������
	float normal_x;
	float normal_y;
	float normal_z;


	Point(float x = 0, float y = 0, float z = 0, float texture_x = 0, float texture_y = 0, float normal_x = 0, float normal_y = 0, float normal_z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->texture_x = texture_x;
		this->texture_y = texture_y;
		this->normal_x = normal_x;
		this->normal_y = normal_y;
		this->normal_z = normal_z;
	}
};


//	����
class Block {
public:
	vector<Point> vertices;
	const float blockSize = 1.0f;
	Point center;

	//	�����������ĵ����ɷ���Ķ���
	Block(Point center) {
		this->center = center;

		//	����
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 1, 0.0f, 0.0f, -1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1, 0.0f, 0.0f, -1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, 0.0f, 0.0f, -1.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 1, 0.0f, 0.0f, -1.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0, 0.0f, 0.0f, -1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, 0.0f, 0.0f, -1.0f));

		//	����
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 0, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0, 0.0f, 0.0f, 1.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 0, 0.0f, 0.0f, 1.0f));

		//	����
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0, -1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, -1.0f, 0.0f, 0.0f));


		//	����
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 1, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 0, 1.0f, 0.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, 1.0f, 0.0f, 0.0f));

		//	����
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1, 0.0f, -1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 0, 1, 0.0f, -1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 0, 0.0f, -1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 0, 0, 0.0f, -1.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z + blockSize / 2, 1, 0, 0.0f, -1.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y + blockSize / 2, center.z - blockSize / 2, 1, 1, 0.0f, -1.0f, 0.0f));

		//	����
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 1, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 0, 1, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Point(center.x - blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 0, 0, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z + blockSize / 2, 1, 0, 0.0f, 1.0f, 0.0f));
		vertices.push_back(Point(center.x + blockSize / 2, center.y - blockSize / 2, center.z - blockSize / 2, 1, 1, 0.0f, 1.0f, 0.0f));
	}
};


float* blockToFloat(vector<Block*> blocks) {
	float* vertices = new float[blocks.size() * 36 * 8];
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < 36; j++) {
			vertices[(i * 36 + j) * 8 + 0] = blocks[i]->vertices[j].x;
			vertices[(i * 36 + j) * 8 + 1] = blocks[i]->vertices[j].y;
			vertices[(i * 36 + j) * 8 + 2] = blocks[i]->vertices[j].z;
			vertices[(i * 36 + j) * 8 + 3] = blocks[i]->vertices[j].texture_x;
			vertices[(i * 36 + j) * 8 + 4] = blocks[i]->vertices[j].texture_y;
			vertices[(i * 36 + j) * 8 + 5] = blocks[i]->vertices[j].normal_x;
			vertices[(i * 36 + j) * 8 + 6] = blocks[i]->vertices[j].normal_y;
			vertices[(i * 36 + j) * 8 + 7] = blocks[i]->vertices[j].normal_z;
		}
	}
	return vertices;
}



