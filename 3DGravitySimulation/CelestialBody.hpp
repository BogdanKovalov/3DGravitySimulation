#pragma once
#ifndef _CELESTIAL_BODY_
#define _CELESTIAL_BODY_

#include "Sphere.h"

#include <glm/glm.hpp>
#include <vector>

typedef std::vector<float> state_type;
class Camera;
class CelestialBody;

extern std::vector<CelestialBody*> AllBodies;

class CelestialBody{
public:
	CelestialBody(float Mass, glm::vec3 Center, glm::vec3 Velocity, glm::vec3 Acceleration, Sphere* Mesh);
	void Update(float DeltaTime, Camera const& Camera);
	Sphere const* GetMesh() const;
	glm::vec3 GetImpulse() const;
	~CelestialBody();
private:
	void UpdateAcceleration();
	void UpdateCenter(float DeltaTime);
	void HandleCollision(CelestialBody const* OtherBody);
	void Draw(Camera const& Camera);
	state_type Solve(float DeltaTime);

	Sphere* mesh_;
	float mass_;
	glm::vec3 center_;
	glm::vec3 velocity_;
	glm::vec3 acceleration_;
};

#endif //_CELESTIAL_BODY_