#pragma once
#ifndef _VISIBLE_OBJECT_HPP_
#define _VISIBLE_OBJECT_HPP_

#include <glm/glm.hpp>

typedef unsigned GLuint;
typedef float GLfloat;
class Shader;
class Camera;

class VisibleObject {
public:
	VisibleObject(GLfloat* Vertices, int NumberOfVertices, GLuint* Indicies, int NumberOfIndices, Shader* shader,
		glm::vec3 Location = glm::vec3(0.0f), glm::vec3 Rotation = glm::vec3(0.0f), glm::vec3 Scale = glm::vec3(1.0f), glm::vec4 Color = glm::vec4(1.0f),
		bool AffectedByLight = true);
	VisibleObject(VisibleObject const& OtherVisibleObject);

	VisibleObject operator= (VisibleObject const& OtherVisibleObject);
	
	virtual void SetLocation(glm::vec3 Location);
	virtual void SetRotation(glm::vec3 Rotation);
	virtual void SetScale(glm::vec3 Scale);
	virtual void SetColor(glm::vec4 Color);

	virtual glm::vec3 GetLocation() const;
	virtual glm::vec3 GetRotation() const;
	virtual glm::vec3 GetScale() const;
	virtual glm::vec4 GetColor() const;
	virtual Shader const * GetShader() const;

	virtual void Draw(Camera const& Camera) const;
protected:
	GLuint VAO_;
	GLuint IBO_;
	Shader* shader_;
	glm::vec3 location_;
	glm::vec3 rotation_;
	glm::vec3 scale_;
	glm::vec4 color_;
	bool bAffectedByLight_;
	int numberOfVertices_;
	int numberOfIndicies_;
};

#endif //_VISIBLE_OBJECT_HPP_