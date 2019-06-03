#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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


//	读取地板的纹理
int getFloorTexture() {
	unsigned int floorTexture;
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
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
	//	释放图片内存
	stbi_image_free(data);
	return floorTexture;
}