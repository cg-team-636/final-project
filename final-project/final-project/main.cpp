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

//	初始化相机
Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
float mouseX = 0.0f;
float mouseY = 0.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//	初始化光源位置
glm::vec3 lightPos(-2.0f, 5.0f, -4.0f);

// 全局 blocks 
vector<vector<Block*>> blocks;
//	初始化blockVAO、blockVBO
unsigned int blockVAO;
unsigned int blockVBO;

int main() {
	//	初始化opengl窗口和配置
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

	//	开启深度测试
	glEnable(GL_DEPTH_TEST);

	//	加载天空纹理
	unsigned int skyBoxTexture = loadSkyBoxTexture();

	//	导入模型
	Model ourModel("model/Tree/Tree.obj");

	//	着色器
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
	
	//	初始化Block
	vector<Block*> floors = createFloor();
	blocks.push_back(floors);

	// 测试阴影的方块
	vector<Block*> testBlocks;
	Block* testBlock1 = new Block(Point(glm::vec3(-2.0f, 5.0f, -4.0f)), "textures/blocks/dirt.png");
	Block* testBlock2 = new Block(Point(glm::vec3(5, 2, -5)), "textures/blocks/dirt.png");
	Block* testBlock3 = new Block(Point(glm::vec3(5, 3, -5)), "textures/blocks/dirt.png");
	Block* testBlock4 = new Block(Point(glm::vec3(0, 1, 0)), "textures/blocks/dirt.png");
	testBlocks.push_back(testBlock1);
	testBlocks.push_back(testBlock2);
	testBlocks.push_back(testBlock3);
	testBlocks.push_back(testBlock4);
	blocks.push_back(testBlocks);

	//	光照参数
	float ambientStrength = 0.5f;
	float diffuseStrength = 1.0f;
	float specularStrength = 1.0f;
	int ShininessStrength = 30;
	glm::vec3 viewPos;

	//	绑定blockVAO
	glGenVertexArrays(1, &blockVAO);
	glBindVertexArray(blockVAO);

	//	设置skyBoxVAO、skyBoxVBO
	unsigned int skyBoxVAO;
	unsigned int skyBoxVBO;
	glGenVertexArrays(1, &skyBoxVAO);
	glBindVertexArray(skyBoxVAO);

	glGenBuffers(1, &skyBoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyBoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyBoxVertices), &skyBoxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// 为渲染的深度贴图创建一个帧缓冲对象
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// 创建一个2D纹理，提供给帧缓冲的深度缓冲使用
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
	// 把生成的深度纹理作为帧缓冲的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//	创建并绑定ImGui
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
		//	计算当前时间和帧间隔时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//	获取键盘输入
		processInput(window);

		//	清除屏幕
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//	清除深度缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		//	创建imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Edit color", &show_window, ImGuiWindowFlags_MenuBar);
		ImGui::Checkbox("Export the model", &export_model);
		ImGui::End();

		//	创建坐标转换矩阵, 将局部坐标变换为标准设备坐标 
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		viewPos = camera.Position;

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		// 计算观察和投影矩阵
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		// 应用到shader
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		simpleDepthShader.setMat4("model", model);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 重置viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	设置shader的uniforms前要开启着色器
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

		//	渲染导入的模型
		if (true) {
			modelShader.use();
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setMat4("model", model);
			ourModel.Draw(modelShader);
		}

		
		//	深度欺骗
		glDepthFunc(GL_LEQUAL);  

		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyBoxShader.use();
		skyBoxShader.setMat4("view", view);
		skyBoxShader.setMat4("projection", projection);

		//	绘制天空盒
		glBindVertexArray(skyBoxVAO);
		//	激活天空盒纹理
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
}

//	鼠标移动
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
}

//	鼠标放缩
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

void renderScene() {
	//	渲染所有的Block
	for (int i = 0; i < blocks.size(); i++) {
		//	将vector<Block>转为vector<Point>
		vector<Point> vertices = blockToPoint(blocks[i]);

		//	绑定blockVAO, blockVBO(若有多个VAO, VBO, 在传入缓冲前要先绑定VAO, VBO)
		glBindVertexArray(blockVAO);
		glGenBuffers(1, &blockVBO);
		glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		//	设置位置属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//	设置纹理属性
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Point, TexCoords));
		glEnableVertexAttribArray(1);

		//	设置法向量属性
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Point, Normal));
		glEnableVertexAttribArray(2);


		//	激活Block纹理并绘制Block
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blocks[i][0]->textureID);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
	}
}
