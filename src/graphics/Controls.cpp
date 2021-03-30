#include "Controls.h"
#include "Utility.hpp"

#include "GLFW/glfw3.h"

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/fast_trigonometry.hpp"

constexpr float FOV = 90.0f;

Control::Control(GLFWwindow* window, const glm::vec3& cameraPosition, const float& hAngle, const float& vAngle, const float& speed, const float& mouseSpeed)
	: m_deltaTime(0), m_window(window), m_cameraPosition(cameraPosition), m_horizontalAngle(hAngle), m_verticalAngle(vAngle), m_speed(speed), m_mouseSpeed(mouseSpeed), m_windowWidth(0), m_windowHeight(0), m_lastTime(0)
{
	UpdateWSize();
	GLCall(glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
	GLCall(glfwSetCursorPos(m_window, m_windowWidth / 2, m_windowHeight / 2));
}

glm::mat4 Control::getProjectionMatrix()
{
	return glm::perspective(glm::radians(FOV), (float)m_windowWidth / (float)m_windowHeight, 0.1f, 100.0f);
}

glm::mat4 Control::getViewMatrix()
{
	glm::vec3 sightPoint = m_cameraPosition + m_direction;
	return glm::lookAt(m_cameraPosition, sightPoint, m_up);
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
	if (m_verticalAngle > 3.14f / 2)
		m_verticalAngle = 3.14f / 2;
	if (m_verticalAngle < -3.14f / 2)
		m_verticalAngle = -3.14f / 2;

}

void Control::UpdateKeyboard()
{
	ComputeVectors();

	/* Moves following one vector */
	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
		m_cameraPosition += m_direction * m_deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
		m_cameraPosition -= m_direction * m_deltaTime * m_speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
		m_cameraPosition += m_right * m_deltaTime * m_speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
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
	//UpdateWSize();
	UpdateMouse();
	UpdateKeyboard();
}

void Control::UpdateWSize()
{
	GLCall(glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight));
}
