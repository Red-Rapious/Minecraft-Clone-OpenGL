#pragma once

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Control
{
private:
	glm::vec3 m_cameraPosition;
	float m_horizontalAngle;
	float m_verticalAngle;
	float m_initialFOV;

	float m_speed;
	float m_mouseSpeed;
	float m_deltaTime;
	double m_lastTime;

	int m_windowWidth, m_windowHeight;

	glm::vec3 m_direction, m_right, m_up;

	GLFWwindow* m_window;
	void UpdateMouse();
	void UpdateKeyboard();
	void UpdateDeltaTime();
	void ComputeVectors();
	

public:
	Control(GLFWwindow* window, glm::vec3 cameraPosition, float hAngle = 3.14f /* Pi radiants */, float vAngle = 0.0f, float iFOV = 250.0f, float speed = 3.0f, float mouseSpeed = 0.1f);
	
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void UpdateInput();
	void UpdateWSize();
};