#include "pfgen.hpp"

using namespace tacoTruck;

void ParticleForceRegistry::updateForces(real duration) {
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++) {
        i->fg->updateForce(i->particle, duration);
    }
}

ParticleGravity::ParticleGravity(const Vector2D &gravity) : gravity(gravity) {}

void ParticleGravity::updateForce(Particle *particle, real duration) {
    if (!particle->hasFiniteMass()) return;
    particle->addForce(gravity * particle->getMass());
}
