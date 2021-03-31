#pragma once

#include "glm/glm.hpp"
#include <iostream>
#include "../game/Chunk.hpp"

class Control
{
private:
	glm::vec3 m_cameraPosition;
	float m_horizontalAngle;
	float m_verticalAngle;

	float m_speed;
	float m_mouseSpeed;
	float m_deltaTime;
	double m_lastTime;

	int m_windowWidth, m_windowHeight;

	glm::vec3 m_direction, m_right, m_up, m_forward;

	GLFWwindow* m_window;
	void UpdateMouse();
	void UpdateKeyboard();
	void UpdateDeltaTime();
	void ComputeVectors();
	

public:
	Control(GLFWwindow* window, const glm::vec3& cameraPosition, const float& hAngle = 3.1415f, const float& vAngle = 0.0f, const float& speed = 15.0f, const float& mouseSpeed = 0.1f);
	
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void UpdateInput();
	void UpdateWSize();

	inline glm::vec3 GetCameraPosition(bool out=false) const { 
		if (out)
			std::cout << "Camera position: x=" << m_cameraPosition.x << " y=" << m_cameraPosition.y << " z=" << m_cameraPosition.z << "\n";
		return m_cameraPosition; 
	};

	glm::vec3 GetAimedBlock(std::vector<std::vector<std::vector<BlockType>>> blocksArray);
};