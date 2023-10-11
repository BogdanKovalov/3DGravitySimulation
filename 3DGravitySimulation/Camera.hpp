#pragma once
#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/glm.hpp>

typedef float GLfloat;
typedef unsigned char GLboolean;
class Shader;

enum eCameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 5.0f;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 45.0f;

class Camera {
	public:
		Camera(glm::vec3 Location, glm::vec3 WorldUp, GLfloat Yaw = YAW, GLfloat Pitch = PITCH);

		glm::mat4 GetViewMatrix() const;
		glm::vec3 GetLocation() const;
		void SetUniforms(Shader const* Shader, bool IsShaderWithLight) const;

		void ProcessKeyboard(eCameraMovement direction, GLfloat DeltaTime);
		void ProcessMouseMovement(GLfloat XOffset, GLfloat YOffset, GLboolean ConstraintPitch = true);
		void ProcessMouseScroll(GLfloat YOffset);
private:
	void UpdateCameraVectors();

	glm::vec3 Location;
	glm::vec3 ForwardVector;
	glm::vec3 UpVector;
	glm::vec3 RightVector;
	glm::vec3 WorldUp;

	GLfloat Yaw;
	GLfloat Pitch;

	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;
};

#endif //_CAMERA_HPP_
