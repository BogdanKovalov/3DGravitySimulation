#include "VisibleObject.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

VisibleObject::VisibleObject(GLfloat* Vertices, int NumberOfVertices, GLuint* Indicies, int NumberOfIndicies, Shader* Shader,
	glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale, glm::vec4 Color, bool AffectedByLight) {
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO_);

	glBindVertexArray(VAO_);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	if (Vertices != nullptr && NumberOfVertices != 0) glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * NumberOfVertices, Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);

	if (NumberOfIndicies != 0) {
		glGenBuffers(1, &IBO_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numberOfIndicies_, Indicies, GL_STATIC_DRAW);
	}

	shader_ = Shader;
	location_ = Location;
	rotation_ = Rotation;
	scale_ = Scale;
	color_ = Color;
	bAffectedByLight_ = AffectedByLight;
	numberOfVertices_ = NumberOfVertices;
	numberOfIndicies_ = NumberOfIndicies;
}

VisibleObject::VisibleObject(VisibleObject const& OtherVisibleObject) {
	VAO_ = OtherVisibleObject.VAO_;
	IBO_ = OtherVisibleObject.IBO_;
	numberOfVertices_ = OtherVisibleObject.numberOfVertices_;
	numberOfIndicies_ = OtherVisibleObject.numberOfIndicies_;
	location_ = OtherVisibleObject.location_;
	rotation_ = OtherVisibleObject.rotation_;
	scale_ = OtherVisibleObject.scale_;
	color_ = OtherVisibleObject.color_;
	bAffectedByLight_ = OtherVisibleObject.bAffectedByLight_;
	if (OtherVisibleObject.shader_) {
		shader_ = OtherVisibleObject.shader_;
	}
}

VisibleObject VisibleObject::operator=(VisibleObject const& OtherVisibleObject) {
	if (this == &OtherVisibleObject) return *this;
	VAO_ = OtherVisibleObject.VAO_;
	IBO_ = OtherVisibleObject.IBO_;
	numberOfVertices_ = OtherVisibleObject.numberOfVertices_;
	numberOfIndicies_ = OtherVisibleObject.numberOfIndicies_;
	location_ = OtherVisibleObject.location_;
	rotation_ = OtherVisibleObject.rotation_;
	scale_ = OtherVisibleObject.scale_;
	color_ = OtherVisibleObject.color_;
	bAffectedByLight_ = OtherVisibleObject.bAffectedByLight_;
	if (OtherVisibleObject.shader_) {
		delete shader_;
		shader_ = OtherVisibleObject.shader_;
	}
	return *this;
}

void VisibleObject::Draw(Camera const& Camera) const {
	shader_->Use();

	Camera.SetUniforms(shader_, bAffectedByLight_);

	GLint transformLoc = glGetUniformLocation(shader_->program, "transform");

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader_->program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(45.0f, 1080.0f / (1920.0f), 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader_->program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, location_);
	transform = glm::scale(transform, glm::vec3(1080.f / (2.f * 1920.f), 1.0f, 1.0f));
	transform = glm::scale(transform, scale_);
	transform = glm::rotate(transform, (GLfloat)(glm::radians(1.f)), rotation_);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (numberOfIndicies_ != 0) glDrawElements(GL_TRIANGLES, numberOfIndicies_, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_);
	glBindVertexArray(0);

}

void VisibleObject::SetLocation(glm::vec3 Location) {
	location_ = Location;
}

void VisibleObject::SetRotation(glm::vec3 Rotation) {
	rotation_ = Rotation;
}

void VisibleObject::SetScale(glm::vec3 Scale) {
	scale_ = Scale;
}

void VisibleObject::SetColor(glm::vec4 Color) {
	color_ = Color;
}

glm::vec3 VisibleObject::GetLocation() const {
	return location_;
}

glm::vec3 VisibleObject::GetRotation() const {
	return rotation_;
}

glm::vec3 VisibleObject::GetScale() const {
	return scale_;
}

glm::vec4 VisibleObject::GetColor() const {
	return color_;
}

Shader const* VisibleObject::GetShader() const {
	return shader_;
}