#include "CelestialBody.hpp"
#include "Camera.hpp"

#include <boost/numeric/odeint.hpp>

typedef boost::numeric::odeint::runge_kutta_dopri5<state_type> rkd5;
typedef boost::numeric::odeint::controlled_runge_kutta<rkd5> crk;

float const G = 6.67f;

std::vector<CelestialBody*> AllBodies = {};

CelestialBody::CelestialBody(float Mass, glm::vec3 Center, glm::vec3 Velocity, glm::vec3 Acceleration, Sphere* Mesh) :
	mass_(Mass), center_(Center), velocity_(Velocity), acceleration_(Acceleration), mesh_(Mesh) {
	try {
		if (mass_ < 0) throw std::invalid_argument("CELESTIALBODY:CONTRUCTOR:MASS CANNOT BE NEGATIVE");
		AllBodies.push_back(this);
		if (mesh_ == nullptr) throw std::invalid_argument("CELESTIALBODY:CONTRUCTOR:MESH IS NULLPTR");
		mesh_->SetLocation(center_);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

CelestialBody::~CelestialBody() {
	auto iterator = std::find_if(AllBodies.begin(), AllBodies.end(), [this](CelestialBody const* Body) {return this == Body; });
	AllBodies.erase(iterator);
	mesh_ = nullptr;
}


void CelestialBody::Draw(Camera const& Camera) {
	if (mesh_ == nullptr) {
		std::cout << "CELESTIALBODY:DRAW:ERROR:MESH IS NULLPTR" << std::endl;
		return;
	}
	mesh_->Draw(Camera);
}

void CelestialBody::Update(float DeltaTime, Camera const& Camera) {
	UpdateAcceleration();
	UpdateCenter(DeltaTime);
	Draw(Camera);
}

void CelestialBody::UpdateAcceleration() {
	float distance = 0.0f;
	acceleration_ = glm::vec3(0.0f);
	for (int i = 0; i < AllBodies.size(); ++i) {
		if (AllBodies[i] == this) continue;
		distance = glm::length(center_ - AllBodies[i]->center_);
		Sphere* mesh = AllBodies[i]->mesh_;
		if (mesh_ == nullptr) continue;
		if (distance < (mesh_->GetRadius() + mesh->GetRadius())) {
			HandleCollision(AllBodies[i]);
			continue;
		}
		acceleration_ += AllBodies[i]->mass_ * (AllBodies[i]->center_ - center_) / ((float)pow(abs(distance), 3));
	}
	acceleration_ *= G;
}

void CelestialBody::UpdateCenter(float DeltaTime) {
	auto x0 = Solve(DeltaTime);
	center_.x = x0[0];
	center_.y = x0[1];
	center_.z = x0[2];
	if (mesh_) mesh_->SetLocation(center_);

	velocity_.x = x0[3];
	velocity_.y = x0[4];
	velocity_.z = x0[5];

	acceleration_.x = x0[6];
	acceleration_.y = x0[7];
	acceleration_.z = x0[8];
}

state_type CelestialBody::Solve(float DeltaTime) {
	state_type x0 = { center_.x, center_.y, center_.z, velocity_.x, velocity_.y, velocity_.z, acceleration_.x, acceleration_.y, acceleration_.z};
	auto System = [](const state_type& x, state_type& dxdt, const double t) {
		dxdt[0] = x[3];
		dxdt[1] = x[4];
		dxdt[2] = x[5]; //equations for center

		dxdt[3] = x[6];
		dxdt[4] = x[7];
		dxdt[5] = x[8]; //equations for velocity
	};
	double t0 = 0.0;
	double t1 = DeltaTime;
	double dt = 0.000001;
	boost::numeric::odeint::integrate_adaptive(crk(), System, x0, t0, t1, dt);
	return x0;
}

void CelestialBody::HandleCollision(CelestialBody const* OtherBody) {
	if (mass_ < OtherBody->mass_) return;
	mass_ += OtherBody->mass_;
	velocity_ = (OtherBody->GetImpulse() + GetImpulse())/(OtherBody->mass_ + mass_);
	OtherBody->~CelestialBody();
}

Sphere const* CelestialBody::GetMesh() const
{
	return mesh_;
}

glm::vec3 CelestialBody::GetImpulse() const {
	return mass_* velocity_;
}