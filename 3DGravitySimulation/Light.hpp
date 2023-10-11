#pragma once
#ifndef _LIGHT_HPP_
#define _LIGHT_HPP_

#include <glm/glm.hpp>

class Light{
public:
	Light(glm::vec4 Color = glm::vec4(1.0), glm::vec3 Location = glm::vec3(0.0f), glm::vec3 Rotation = glm::vec3(0.0f), glm::vec3 Scale = glm::vec3(1.0f));

	virtual void SetLocation(glm::vec3 Location);
	virtual void SetRotation(glm::vec3 Rotation);
	virtual void SetScale(glm::vec3 Scale);
	virtual void SetColor(glm::vec4 Color);

	virtual glm::vec3 GetLocation() const;
	virtual glm::vec3 GetRotation() const;
	virtual glm::vec3 GetScale() const;
	virtual glm::vec4 GetColor() const;

private:
	glm::vec4 color_;
	glm::vec3 location_;
	glm::vec3 rotation_;
	glm::vec3 scale_;
};

#endif //_LIGHT_HPP_
