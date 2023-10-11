#pragma once
#ifndef _SPHERE_H_
#define _SPHERE_H_
typedef unsigned int  GLuint;
typedef float GLfloat;
class Camera;

#include "VisibleObject.hpp"

class Sphere : public VisibleObject
{
public:
	Sphere(unsigned Slices, unsigned Stacks, float Radius, Shader* Shader,
		glm::vec3 Location = glm::vec3(0.0f), glm::vec3 Rotation = glm::vec3(0.0f), glm::vec3 Scale = glm::vec3(1.0f), bool AffectedByLight = true);
	Sphere(Sphere const& OtherSphere);
	Sphere operator= (Sphere const& OtherSphere);
	void Draw(Camera const& camera) const override;
	bool IsAffectedByLight() const;
	void SetScale(glm::vec3 Scale) override;

	float GetRadius() const;
private:
	void CalculateIndices(unsigned Slices, unsigned Stacks);
	void SetupVAO(GLfloat* Vertices, int const NumberOfVertices);
	float radius_;
};

#endif //_SPHERE_H_
