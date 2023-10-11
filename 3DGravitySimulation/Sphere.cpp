#include "Sphere.h"
#include "Math.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <cmath>
Sphere::Sphere(unsigned Slices, unsigned Stacks, float Radius, Shader* Shader,
	glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale, bool AffectedByLight):
	VisibleObject(nullptr, 0, nullptr, 0, Shader, Location, Rotation, Scale, glm::vec4(1.0f), AffectedByLight), radius_(Radius) {
	try {
		if (Slices == 0 || Stacks == 0) throw std::exception("ERROR::SPHERE::SLICES OR STACKS IS 0");
		std::vector<glm::vec3> vertices = { glm::vec3(0,0,Radius) ,glm::vec3(0,0,Radius) };
		float Theta = 0.f;
		float Phi = 0.f;
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		for (int i = 1; i < Slices; ++i) {
			Theta = glm::radians(180.f / Slices * i);
			for (int j = 0; j < Stacks; ++j) {
				Phi = glm::radians(360.f / Stacks * j);
				x = RoundSmallToZero(Radius * glm::sin(Theta) * glm::cos(Phi));
				y = RoundSmallToZero(Radius * glm::sin(Theta) * glm::sin(Phi));
				z = RoundSmallToZero(Radius * glm::cos(Theta));
				vertices.push_back(glm::vec3(x, y, z));
				vertices.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
		}
		vertices.push_back(glm::vec3(0.0f, 0.0f, -Radius));
		vertices.push_back(glm::vec3(0.0f, 0.0f, -Radius));

		const int size = vertices.size();
		GLfloat* verticesForBuffer = new GLfloat[size * 3];
		for (int i = 0; i < size; ++i) {
			verticesForBuffer[i * 3] = vertices[i].x;
			verticesForBuffer[i * 3 + 1] = vertices[i].y;
			verticesForBuffer[i * 3 + 2] = vertices[i].z;
		}

		SetupVAO(verticesForBuffer, size);

		CalculateIndices(Slices, Stacks);

		delete[] verticesForBuffer;
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
}

Sphere::Sphere(Sphere const& OtherSphere) : VisibleObject(nullptr, 0, nullptr, 0, OtherSphere.shader_) {
	VAO_ = OtherSphere.VAO_;
	IBO_ = OtherSphere.IBO_;
	location_ = OtherSphere.location_;
	rotation_ = OtherSphere.rotation_;
	scale_ = OtherSphere.scale_;
	numberOfIndicies_ = OtherSphere.numberOfIndicies_;
	numberOfVertices_ = OtherSphere.numberOfVertices_;
	if (OtherSphere.shader_) {
		shader_ = OtherSphere.shader_;
	}
}

Sphere Sphere::operator=(Sphere const& OtherSphere) {
	if (this == &OtherSphere) return *this;
	VAO_ = OtherSphere.VAO_;
	IBO_ = OtherSphere.IBO_;
	location_ = OtherSphere.location_;
	rotation_ = OtherSphere.rotation_;
	scale_ = OtherSphere.scale_;
	numberOfIndicies_ = OtherSphere.numberOfIndicies_;
	numberOfVertices_ = OtherSphere.numberOfVertices_;
	if (OtherSphere.shader_) {
		shader_ = OtherSphere.shader_;
	}
	return *this;
}

void Sphere::Draw(Camera const& camera) const {
	if (shader_ == nullptr) {
		std::cout << "SPHERE:DRAW:ERROR:SHADER_ IS NULLPTR" << std::endl;
		return;
	}
	shader_->Use();

	camera.SetUniforms(shader_, bAffectedByLight_);

	GLint modelLoc = glGetUniformLocation(shader_->program, "model");

	GLint colorLoc = glGetUniformLocation(shader_->program, "objectColor");
	glUniform3f(colorLoc, color_.r, color_.g, color_.b);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), location_);
	transform = glm::scale(transform, scale_);
	if (rotation_ != glm::vec3(0.0f)) {
		transform = glm::rotate(transform, glm::radians(glm::length(rotation_)), rotation_);
	}
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
	glDrawElements(GL_TRIANGLES, numberOfIndicies_,GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::CalculateIndices(unsigned Slices, unsigned Stacks) {
	std::vector<unsigned> indices;
	int k1 = 0, k2 = 1;
	for (int i = 0; i < Stacks; ++i) {
		for (int j = 0; j < Slices; ++j, ++k1, ++k2) {
			if (i == 0 && j != 0) --k1;
			if (i == Stacks - 1) {
				--k2;
			}
			if (j == (Slices - 1)) {
				if (i != Stacks - 1) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k2 - j);
				}
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k1 - j);
					indices.push_back(k2 - j);
				}	//triangles to finish circle
			}
			else {
				if (i != Stacks - 1) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k2 + 1); //first triangle
				}
				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k1 + 1);
					indices.push_back(k2 + 1); //second triangle
				}
			}
		}
	}
	indices.pop_back();
	indices.push_back(Slices * (Stacks - 1) + 1);
	numberOfIndicies_ = indices.size();
	GLuint* indiciesArray = new GLuint[numberOfIndicies_];
	for (int i = 0; i < numberOfIndicies_; ++i) {
		indiciesArray[i] = indices[i];
	}
	glGenBuffers(1, &IBO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numberOfIndicies_, indiciesArray, GL_STATIC_DRAW);
	delete[] indiciesArray;
}

void Sphere::SetupVAO(GLfloat* Vertices, int const NumberOfVertices) {
	GLuint VBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumberOfVertices, Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

bool Sphere::IsAffectedByLight() const {
	return bAffectedByLight_;
}

void Sphere::SetScale(glm::vec3 Scale) {
	try {
		if (Scale.x != Scale.y || Scale.x != Scale.z) throw std::invalid_argument("SPHERE:ERROR:SETSCALE:SCALE COMPONENT ISN`T EQUAL");
		scale_ = Scale;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

float Sphere::GetRadius() const {
	return scale_.x * radius_;
}