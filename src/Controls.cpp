#include "Controls.h"
#include "Renderer.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>


Control::Control(GLFWwindow * window, glm::vec3 cameraPosition, float hAngle, float vAngle, float iFOV, float speed, float mouseSpeed)
	: m_deltaTime(0), m_window(window), m_cameraPosition(cameraPosition), m_horizontalAngle(hAngle), m_verticalAngle(vAngle), m_initialFOV(iFOV), m_speed(speed), m_mouseSpeed(mouseSpeed), m_windowWidth(0), m_windowHeight(0)
{
	//glfwSetScrollCallback(window, GetMouseWheel);
	UpdateWSize();
	GLCall(glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN));
}

glm::mat4 Control::getProjectionMatrix() const
{
	return glm::mat4(1.0f);
}

glm::mat4 Control::getViewMatrix() const
{
	

	return glm::mat4(1.0f);
}

void Control::UpdateMouse()
{
	/* Store the mouse position and reset it to the center of the window*/
	double xpos, ypos; // mouse position
	GLCall(glfwGetCursorPos(m_window, &xpos, &ypos));
	
	glfwSetCursorPos(m_window, m_windowWidth /2, m_windowHeight /2);

	/* Compute new orientation */
	float deltaTime = 1;
	m_horizontalAngle += m_mouseSpeed * m_deltaTime * float(m_windowWidth / 2 - xpos);
	m_verticalAngle += m_mouseSpeed * m_deltaTime * float(m_windowHeight / 2 - ypos);

	/* Update FOV with the mouse wheel*/
	//float FoV = m_initialFOV - 5 * GetMouseWheel();

}

void Control::UpdateKeyboard()
{
	/* Computes vectors we need to update the position */
	glm::vec3 direction
	(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	/* Moves following one vector */
	// Move forward
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_cameraPosition += direction * m_deltaTime * m_speed;
	}
	// Move backward
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_cameraPosition -= direction * m_deltaTime * m_speed;
	}
	// Strafe right
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_cameraPosition += right * m_deltaTime * m_speed;
	}
	// Strafe left
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_cameraPosition -= right * m_deltaTime * m_speed;
	}
}

void Control::UpdateDeltaTime()
{
	double currentTime = glfwGetTime();
	m_deltaTime = float(currentTime - m_lastTime);
	m_lastTime = currentTime;
}

void Control::UpdateInput()
{
	UpdateMouse();
	UpdateKeyboard();
}

void Control::UpdateWSize()
{
	glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
}

void Control::GetMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	if (window == m_window)
	{
		std::cout << xoffset << "   " << yoffset << "\n";
	}
}