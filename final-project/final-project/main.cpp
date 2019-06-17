#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "camera.h"
#include "floor.h"
#include "skyBox.h"
#include "model.h"

using namespace std;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderScene();

//	��ʼ�����
Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
float mouseX = 0.0f;
float mouseY = 0.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//	��ʼ����Դλ��
glm::vec3 lightPos(-1.0f, 5.0f, 2.0f);

// ȫ�� blocks 
vector<vector<Block*>> blocks;
vector<Block*> virtualBlock;

//	��ʼ��blockVAO��blockVBO
unsigned int blockVAO;
unsigned int blockVBO;

int main() {
	

	//	��ʼ��opengl���ں�����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//	������Ȳ���
	glEnable(GL_DEPTH_TEST);

	//	�����������
	unsigned int skyBoxTexture = loadSkyBoxTexture();

	//	����ģ��
	Model ourModel("model/Tree/Tree.obj");

	//	��ɫ��
	/*Shader blockShader("block.vs", "block.fs");
	blockShader.use();
	blockShader.setInt("floorTexture", 0);*/
	Shader blockShader("shadowMappingVs.vs", "shadowMappingFs.fs");
	blockShader.use();
	blockShader.setInt("ourTexture", 0);
	blockShader.setInt("shadowMap", 1);

	Shader simpleDepthShader("shadowMappingDepthVs.vs", "shadowMappingDepthFs.fs");

	Shader debugDepthQuad("debugQuadVs.vs", "debugQuadDepthFs.fs");
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);

	Shader skyBoxShader("skyBox.vs", "skyBox.fs");
	skyBoxShader.use();
	skyBoxShader.setInt("skyBoxTexture", 0);

	Shader modelShader("model.vs", "model.fs");
	modelShader.use();
	
	//	��ʼ��Block
	blocks.push_back(virtualBlock);

	// ��ľ
	vector<Block*> trees = createTree();
	blocks.push_back(trees);

	// ��Ҷ
	vector<Block*> TreeLeaves = createTreeLeaves();
	blocks.push_back(TreeLeaves);

	// �̲ݵ�
	vector<Block*> greenFloors = createGreenFloors();
	blocks.push_back(greenFloors);

	// ˮ
	vector<Block*> waters = createWaters();
	blocks.push_back(waters);

	// ���
	vector<Block*> DirtFloors = createDirtFloors();
	blocks.push_back(DirtFloors);

	// �ذ�
	vector<Block*> floors = createFloor();
	blocks.push_back(floors);

	// ������Ӱ�ķ���
	vector<Block*> testBlocks;
	Block* testBlock1 = new Block(Point(glm::vec3(-1.0f, 5.0f, 2.0f)), "textures/blocks/dirt.png");
	Block* testBlock2 = new Block(Point(glm::vec3(3, 1, -3)), "textures/blocks/dirt.png");
	Block* testBlock3 = new Block(Point(glm::vec3(3, 2, -3)), "textures/blocks/dirt.png");
	Block* testBlock4 = new Block(Point(glm::vec3(0, 1, 0)), "textures/blocks/dirt.png");
	testBlocks.push_back(testBlock1);
	testBlocks.push_back(testBlock2);
	testBlocks.push_back(testBlock3);
	testBlocks.push_back(testBlock4);
	blocks.push_back(testBlocks);

	//	���ղ���
	float ambientStrength = 0.7f;
	float diffuseStrength = 0.7f;
	float specularStrength = 0.7f;
	int ShininessStrength = 64;
	glm::vec3 viewPos;

	//	��blockVAO
	glGenVertexArrays(1, &blockVAO);
	glBindVertexArray(blockVAO);

	//	����skyBoxVAO��skyBoxVBO
	unsigned int skyBoxVAO;
	unsigned int skyBoxVBO;
	glGenVertexArrays(1, &skyBoxVAO);
	glBindVertexArray(skyBoxVAO);

	glGenBuffers(1, &skyBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Ϊ��Ⱦ�������ͼ����һ��֡�������
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// ����һ��2D�����ṩ��֡�������Ȼ���ʹ��
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// �����ɵ����������Ϊ֡�������Ȼ���
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	��������ImGui
	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	bool show_window = true;
	bool export_model = false;


	while (!glfwWindowShouldClose(window)) {
		//	���㵱ǰʱ���֡���ʱ��
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//	��ȡ��������
		processInput(window);

		//	�����Ļ
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//	�����Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//	����imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Edit color", &show_window, ImGuiWindowFlags_MenuBar);
		ImGui::Checkbox("Export the model", &export_model);
		ImGui::End();

		//	��������ת������, ���ֲ�����任Ϊ��׼�豸���� 
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		viewPos = camera.Position;

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		// ����۲��ͶӰ����
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		// Ӧ�õ�shader
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		simpleDepthShader.setMat4("model", model);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ����viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	����shader��uniformsǰҪ������ɫ��
		blockShader.use();
		blockShader.setMat4("model", model);
		blockShader.setMat4("view", view);
		blockShader.setMat4("projection", projection);
		blockShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		blockShader.setFloat("ambientStrength", ambientStrength);
		blockShader.setFloat("diffuseStrength", diffuseStrength);
		blockShader.setFloat("specularStrength", specularStrength);
		blockShader.setInt("ShininessStrength", ShininessStrength);
		blockShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		blockShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		blockShader.setVec3("lightPos", lightPos);
		blockShader.setVec3("viewPos", viewPos);	

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene();

		//	��Ⱦ�����ģ��
		if (true) {
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, glm::vec3(5, 0, -5));
			modelShader.use();
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setMat4("model", model1);
			ourModel.Draw(modelShader);
		}

		
		//	�����ƭ
		glDepthFunc(GL_LEQUAL);  

		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyBoxShader.use();
		skyBoxShader.setMat4("view", view);
		skyBoxShader.setMat4("projection", projection);

		//	������պ�
		glBindVertexArray(skyBoxVAO);
		//	������պ�����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDepthFunc(GL_LESS);


		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &blockVAO);
	glDeleteBuffers(1, &blockVBO);
	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteBuffers(1, &skyBoxVBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Block* nowBlock = NULL;
bool firstPlaceFlag = true;
bool startPlacingFlag = false;
glm::vec3 newCenter;





bool checkCollision2D(float x1, float x2, float y1, float y2, float xCenter, float yCenter) {
	if (x1 - x2 == 0) {
		if (x1 < xCenter + 0.5 && x1 > xCenter - 0.5) return true;
		return false;
	}
	else {
		float inv = 1 / (x1 - x2);
		float k = (y1 - y2) * inv;
		float b = (x1 * y2 - x2 * y1) * inv;
		float xF = xCenter + 0.5;
		float t0 = k * (xCenter - 0.5) + b;
		float t1 = k * (xCenter + 0.5) + b;
		if (t0 > t1) {
			float tmp = t0;
			t1 = tmp;
			t0 = t1;
		}
		if (t0 <= yCenter - 0.5) {
			if (t1 > yCenter - 0.5) return true;
			return false;
		}
		else if (t0 < yCenter + 0.5) {
			return true;
		}
		else {
			return false;
		}
	}
}



Block* checkCollision(glm::vec3 start, glm::vec3 end, glm::vec3 towards) {

	Block* hit = NULL;
	float x1 = start.x;
	float x2 = end.x;
	float y1 = start.y;
	float y2 = end.y;
	float z1 = start.x;
	float z2 = end.z;

	for (int i = 1; i < blocks.size(); i++) {
		for (Block* b : blocks[i]) {

			glm::vec3 c = b->center.Position;
			glm::vec3 v1 = b->center.Position - start;

			
			if (glm::dot(v1, towards) < 0) continue;

			if (checkCollision2D(x1, x2, y1, y2, c.x, c.y) && checkCollision2D(y1, y2, z1, z2, c.y, c.z)) {
				if (b != nowBlock) {
					if (hit == NULL) {
						hit = b;
					}
					else {
						if (glm::dot(v1, towards) < glm::dot(hit->center.Normal - start, towards)) {
							hit = b;
						}
					}
				}
				
			}
		}
	}

	if (hit) {
		glm::vec3 p = hit->center.Position;
		float t0 = start.x - p.x;
		float t1 = start.y - p.y;
		float t2 = start.z - p.z;
		float a0 = glm::abs(t0);
		float a1 = glm::abs(t1);
		float a2 = glm::abs(t2);
		if (a0 < a1) {
			if (a1 < a2) {
				if (t2 > 0) {
					newCenter = glm::vec3(p.x, p.y, p.z + 1);
				}
				else {
					newCenter = glm::vec3(p.x, p.y, p.z - 1);
				}
			}
			else {
				if (t1 > 0) {
					newCenter = glm::vec3(p.x, p.y + 1, p.z);
				}
				else {
					newCenter = glm::vec3(p.x, p.y - 1, p.z);
				}
			}
		}
		else {
			if (t0 > 0) {
				newCenter = glm::vec3(p.x + 1, p.y, p.z);
			}
			else {
				newCenter = glm::vec3(p.x - 1, p.y, p.z);
			}
		}
	}
	return hit;
}

void placingCube() {

	glm::vec3 camPos = camera.Position;

	glm::vec3 towards = glm::normalize(camera.Front);

	float nearUnit = 3;
	float farUnit = 15;

	glm::vec3 startPos = camPos + towards * nearUnit;
	glm::vec3 endPos = camPos + towards * farUnit;

	Block* newBlock = checkCollision(startPos, endPos, towards);
	
	if (newBlock) {
		nowBlock = newBlock;
		if (!firstPlaceFlag) {
			blocks[0].pop_back();
		}
		firstPlaceFlag = false;
		
		blocks[0].push_back(new Block(newCenter, "textures/blocks/dirt.png"));
	}
}

//	����ƶ�
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse){
		mouseX = xpos;
		mouseY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - mouseX;
	float yoffset = mouseY - ypos;

	mouseX = xpos;
	mouseY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

	if (startPlacingFlag) {
		placingCube();
	}
}

//	������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void renderScene() {
	//	��Ⱦ���е�Block
	
	for (int i = 0; i < blocks.size(); i++) {
		//	��vector<Block>תΪvector<Point>
		if (!blocks[i].empty()) {
			vector<Point> vertices = blockToPoint(blocks[i]);
			//	��blockVAO, blockVBO(���ж��VAO, VBO, �ڴ��뻺��ǰҪ�Ȱ�VAO, VBO)
			glBindVertexArray(blockVAO);
			glGenBuffers(1, &blockVBO);
			glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			//	����λ������
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			//	������������
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Point, TexCoords));
			glEnableVertexAttribArray(1);

			//	���÷���������
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Point, Normal));
			glEnableVertexAttribArray(2);


			//	����Block��������Block
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blocks[i][0]->textureID);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);
		}
	}
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		startPlacingFlag = !startPlacingFlag;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (startPlacingFlag) {
			nowBlock = NULL;
			firstPlaceFlag = true;
			if (!blocks[0].empty()) {
				Block* b = blocks[0].back();
				blocks[0].pop_back();
				blocks[1].push_back(b);
			}
		}
	}
}