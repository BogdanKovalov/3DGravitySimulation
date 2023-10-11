#define GLEW_STATIC 
#include <GL/glew.h> //connecting OpenGL and OpenGLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //connecting OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Sphere.h"
#include "Light.hpp"
#include "Camera.hpp"
#include "CelestialBody.hpp"

#include <iostream>
#include <vector>

Camera camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
double DeltaTime = 0.0;
bool IsFirstMouseCall = true;
GLfloat LastX = 0.0f;
GLfloat LastY = 0.0f;
bool keys[1024];
std::vector<CelestialBody*> Bodies;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

GLFWwindow* SystemInitialization() {
	if (glfwInit() != GL_TRUE) { //initializing glfw system and setting up options for window
		std::cout << "Failed to initialize GLFW system" << std::endl;
		return nullptr;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "Gravity Simulation", NULL, NULL); //creating window
	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW system" << std::endl;
		return nullptr;
	}
	int height = 0, width = 0;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	return window;
}

int main() {
	GLFWwindow* window = SystemInitialization();
	
	Shader shader(".\\Shaders\\vertex.v", ".\\Shaders\\fragment.frag");
	Shader lightShader(".\\Shaders\\vertexLight.v", ".\\Shaders\\fragmentLight.frag");

	glEnable(GL_DEPTH_TEST);
	Sphere a(32, 32, 1, & shader);
	Sphere b(32, 32, 1, &lightShader, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), false);
	Light light (glm::vec4(1.0f));

	CelestialBody second(200.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), &b);
	CelestialBody first(20.0f, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f, 10.0f), glm::vec3(0.0f), &a);
	CelestialBody third(20.0f, glm::vec3(-12.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(0.0f), &a);
	CelestialBody fourth(20.0f, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(12.0f, 0.0f, 0.0f), glm::vec3(0.0f), &a);

	double lastFrameTime = 0;
	glfwSetKeyCallback(window, &key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window,scroll_callback);

	glm::vec3 startImpulse = glm::vec3(0.0f);

	for (auto Body : AllBodies) {
		startImpulse += Body->GetImpulse();
	}

	std::cout << "Start Impulse: " << glm::length(startImpulse) << std::endl;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DeltaTime = glfwGetTime() - lastFrameTime;
		lastFrameTime = glfwGetTime();

		light.SetLocation(second.GetMesh()->GetLocation());

		glUniform3f(glGetUniformLocation(a.GetShader()->program, "lightPos"), light.GetLocation().x, light.GetLocation().y, light.GetLocation().z);
		glUniform3f(glGetUniformLocation(a.GetShader()->program, "lightColor"), light.GetColor().r, light.GetColor().g, light.GetColor().b);

		std::cout << AllBodies.size() << std::endl;

		for (int i = 0; i < AllBodies.size(); ++i) {
			AllBodies[i]->Update(DeltaTime, camera);
		}

		glfwSwapBuffers(window);
	}

	glm::vec3 endImpulse = glm::vec3(0.0f);
	for (auto Body : AllBodies) {
		endImpulse += Body->GetImpulse();
	}
	std::cout << "End Impulse: " << glm::length(endImpulse) << std::endl;

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* Window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(Window, GL_TRUE);
	if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(eCameraMovement::FORWARD, DeltaTime);
	if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(eCameraMovement::BACKWARD, DeltaTime);
	if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(eCameraMovement::RIGHT, DeltaTime);
	if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(eCameraMovement::LEFT, DeltaTime);
}

void mouse_callback(GLFWwindow* Window, double xpos, double ypos) {
	if (IsFirstMouseCall) {
		LastX = xpos;
		LastY = ypos;
		IsFirstMouseCall = false;
	}
	camera.ProcessMouseMovement(xpos - LastX, ypos - LastY);
	LastX = xpos;
	LastY = ypos;
}

void scroll_callback(GLFWwindow* Window, double XOffset, double YOffset) {
	camera.ProcessMouseScroll(YOffset);
}