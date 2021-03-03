#include "Controls.h"
#include "Renderer.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/fast_trigonometry.hpp"


void GetMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	//std::cout << yoffset << "\n";
}

Control::Control(GLFWwindow * window, glm::vec3 cameraPosition, float hAngle, float vAngle, float iFOV, float speed, float mouseSpeed)
	: m_deltaTime(0), m_window(window), m_cameraPosition(cameraPosition), m_horizontalAngle(hAngle), m_verticalAngle(vAngle), m_initialFOV(iFOV), m_speed(speed), m_mouseSpeed(mouseSpeed), m_windowWidth(0), m_windowHeight(0)
{
	//glfwSetScrollCallback(window, ::GetMouseWheel);
	UpdateWSize();
	GLCall(glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
	GLCall(glfwSetCursorPos(m_window, m_windowWidth / 2, m_windowHeight / 2));
}

glm::mat4 Control::getProjectionMatrix()
{
	return glm::perspective(glm::radians(m_initialFOV), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);
}

glm::mat4 Control::getViewMatrix()
{
	//glm::vec3 center = m_cameraPosition + m_direction;
	glm::vec3 center = glm::vec3(0, 0, 0);
	return glm::lookAt(m_cameraPosition, center, m_up);
}

void Control::UpdateMouse()
{
	/* Store the mouse position and reset it to the center of the window*/
	double xpos, ypos; // mouse position
	GLCall(glfwGetCursorPos(m_window, &xpos, &ypos));
	
	GLCall(glfwSetCursorPos(m_window, m_windowWidth /2, m_windowHeight /2));

	/* Compute new orientation */
	m_horizontalAngle += m_mouseSpeed * m_deltaTime * float(m_windowWidth / 2 - xpos);
	m_verticalAngle += m_mouseSpeed * m_deltaTime * float(m_windowHeight / 2 - ypos);

	/* Update FOV with the mouse wheel*/
	//float FoV = m_initialFOV - 5 * GetMouseWheel();

}

void Control::UpdateKeyboard()
{
	ComputeVectors();

	/* Moves following one vector */
	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_cameraPosition += m_direction * m_deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_cameraPosition -= m_direction * m_deltaTime * m_speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_cameraPosition += m_right * m_deltaTime * m_speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_cameraPosition -= m_right * m_deltaTime * m_speed;
	}
}

void Control::UpdateDeltaTime()
{
	double currentTime = glfwGetTime();
	m_deltaTime = float(currentTime - m_lastTime);
	m_lastTime = currentTime;
}

void Control::ComputeVectors()
{
	/* Computes vectors we need to update the position */
	m_direction = glm::vec3(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	m_right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	m_up = glm::cross(m_right, m_direction);
}

void Control::UpdateInput()
{
	UpdateDeltaTime();
	UpdateMouse();
	UpdateKeyboard();

	int radius = 3;
	double currentTime = glfwGetTime();
	m_cameraPosition = glm::vec3(0, 0, 0) + glm::vec3(radius * glm::cos(currentTime*2), 0, radius * glm::sin(currentTime*2));
}

void Control::UpdateWSize()
{
	GLCall(glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight));
}
