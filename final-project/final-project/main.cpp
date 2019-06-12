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

//	��ʼ�����
Camera camera(glm::vec3(0.0f, 10.0f, 10.0f));
float mouseX = 0.0f;
float mouseY = 0.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;


//	��ʼ����Դλ��
glm::vec3 lightPos(10.0f, -5.0f, 5.0f);


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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
	Shader blockShader("block.vs", "block.fs");
	blockShader.use();
	blockShader.setInt("floorTexture", 0);

	Shader skyBoxShader("skyBox.vs", "skyBox.fs");
	skyBoxShader.use();
	skyBoxShader.setInt("skyBoxTexture", 0);

	Shader modelShader("model.vs", "model.fs");
	modelShader.use();
	
	//	��ʼ��Block
	vector<vector<Block*>> blocks;
	vector<Block*> floors = createFloor();
	blocks.push_back(floors);


	//	���ղ���
	float ambientStrength = 0.5f;
	float diffuseStrength = 1.0f;
	float specularStrength = 1.0f;
	int ShininessStrength = 30;
	glm::vec3 viewPos;

	//	��ʼ��blockVAO��blockVBO
	unsigned int blockVAO;
	unsigned int blockVBO;

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
		glClear(GL_COLOR_BUFFER_BIT);
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

		//	����shader��uniformsǰҪ������ɫ��
		blockShader.use();
		blockShader.setFloat("ambientStrength", ambientStrength);
		blockShader.setFloat("diffuseStrength", diffuseStrength);
		blockShader.setFloat("specularStrength", specularStrength);
		blockShader.setInt("ShininessStrength", ShininessStrength);

		blockShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		blockShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		blockShader.setVec3("lightPos", lightPos);
		blockShader.setVec3("viewPos", viewPos);

		blockShader.setMat4("model", model);
		blockShader.setMat4("view", view);
		blockShader.setMat4("projection", projection);


		//	��Ⱦ���е�Block
		for (int i = 0; i < blocks.size(); i++) {
			//	��vector<Block>תΪvector<Point>
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

		//	��Ⱦ�����ģ��
		if (true) {
			modelShader.use();
			modelShader.setMat4("projection", projection);
			modelShader.setMat4("view", view);
			modelShader.setMat4("model", model);
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
}

//	������
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}


