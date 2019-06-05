#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
using namespace std;


//	��
class Point {
public:
	//	�����ά����
	glm::vec3 Position;

	//	��Ĳ�������
	glm::vec2 TexCoords;

	//	��ķ���������
	glm::vec3 Normal;


	Point(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2 TexCoords = glm::vec2(0.0f, 0.0f), glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f)) {
		this->Position = Position;
		this->TexCoords = TexCoords;
		this->Normal = Normal;
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
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));

		//	����
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));


		//	����
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));



		//	����
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));


		//	����
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y + blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


		//	����
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x - blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z + blockSize / 2), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		vertices.push_back(Point(glm::vec3(center.Position.x + blockSize / 2, center.Position.y - blockSize / 2, center.Position.z - blockSize / 2), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	}
};


vector<Point> blockToPoint(vector<Block*> blocks) {
	vector<Point> vertices;
	for (int i = 0; i < blocks.size(); i++) {
		vertices.insert(vertices.end(), blocks[i]->vertices.begin(), blocks[i]->vertices.end());
	}

	return vertices;
}

//	��������
unsigned int loadBlockTexture(char const* path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	//	�ͷ�ͼƬ�ڴ�
	stbi_image_free(data);
	return textureID;
}


