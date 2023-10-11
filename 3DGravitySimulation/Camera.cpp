#include "Camera.hpp"
#include "Shader.hpp"

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Camera::Camera(glm::vec3 Location, glm::vec3 WorldUp, GLfloat Yaw, GLfloat Pitch) :
	ForwardVector(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
	this->Location = Location;
	this->WorldUp = WorldUp;
	this->Yaw = Yaw;
	this->Pitch = Pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(Location, Location + ForwardVector, UpVector);
}

glm::vec3 Camera::GetLocation() const {
	return Location;
}

void Camera::SetUniforms(Shader const* Shader, bool IsShaderWithLight) const {
	glUniformMatrix4fv(glGetUniformLocation(Shader->program, "view"), 1, GL_FALSE, glm::value_ptr(GetViewMatrix()));

	if (IsShaderWithLight) glUniform3f(glGetUniformLocation(Shader->program, "viewPos"), Location.x, Location.y, Location.z);

	glm::mat4 projection = glm::perspective(glm::radians(Zoom), 1920.0f / (1080.0f), 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(Shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::ProcessKeyboard(eCameraMovement Direction, GLfloat DeltaTime) {
	GLfloat Velocity = MovementSpeed * DeltaTime;
	if (Direction == FORWARD) {
		Location += ForwardVector * Velocity;
	}
	if (Direction == BACKWARD) {
		Location -= ForwardVector * Velocity;
	}
	if (Direction == RIGHT) {
		Location += RightVector * Velocity;
	}
	if (Direction == LEFT) {
		Location -= RightVector * Velocity;
	}
}

void Camera::ProcessMouseMovement(GLfloat XOffset, GLfloat YOffset, GLboolean ConstraintPitch) {
	Yaw += XOffset * MouseSensitivity;
	Pitch -= YOffset * MouseSensitivity;

	if (ConstraintPitch) {
		if (Pitch > 89.0f) {
			Yaw = 89.0f;
		}
		if (Pitch < -89.0f) {
			Pitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat YOffset) {
	if (Zoom >= 1.0f && Zoom <= 45.0f) {
		Zoom -= YOffset;
	}
	if (Zoom < 1.0f) {
		Zoom = 1.0f;
	}
	if (Zoom > 45.0f) {
		Zoom = 45.0f;
	}
}

void Camera::UpdateCameraVectors() {
	glm::vec3 Front = glm::vec3(0.0f);
	Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	ForwardVector = glm::normalize(Front);
	RightVector = glm::normalize(glm::cross(ForwardVector, WorldUp));
	UpVector = glm::normalize(glm::cross(RightVector, ForwardVector));
}