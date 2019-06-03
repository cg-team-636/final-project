#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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


//	��ȡ�ذ������
int getFloorTexture() {
	unsigned int floorTexture;
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/blocks/dirt.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	//	�ͷ�ͼƬ�ڴ�
	stbi_image_free(data);
	return floorTexture;
}