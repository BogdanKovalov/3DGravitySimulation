#include "Light.hpp"


Light::Light(glm::vec4 Color, glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale): color_(Color), location_(Location), rotation_(Rotation), scale_(Scale) {

};

void Light::SetLocation(glm::vec3 Location) {
	location_ = Location;
}

void Light::SetRotation(glm::vec3 Rotation) {
	rotation_ = Rotation;
}

void Light::SetScale(glm::vec3 Scale) {
	scale_ = Scale;
}

void Light::SetColor(glm::vec4 Color) {
	color_ = Color;
}

glm::vec3 Light::GetLocation() const {
	return location_;
}

glm::vec3 Light::GetRotation() const {
	return rotation_;
}

glm::vec3 Light::GetScale() const {
	return scale_;
}

glm::vec4 Light::GetColor() const {
	return color_;
}