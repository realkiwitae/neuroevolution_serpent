#include "Camera.h"
#include "../Engine/Utils/CommonValues.h"
#include <iostream>
Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(long* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

    if(CHECK_BIT(*keys,GLFW_KEY_W)){
        position += front*velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_S)){
        position -= front*velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_A)){
        position -= right*velocity;
    }
    if(CHECK_BIT(*keys,GLFW_KEY_D)){
        position += right*velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}
glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	// std::cout <<"pos("<<position.x << "," << position.y<<" " << position.z << " ";
	// std::cout <<"yaw " << yaw << " pitch " << pitch << std::endl;
}


Camera::~Camera()
{
}
