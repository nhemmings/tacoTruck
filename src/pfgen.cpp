#include "pfgen.hpp"

using namespace tacoTruck;

/*******************************************************************************************************************//**
 *  PARTICLE FORCE REGISTRY
***********************************************************************************************************************/

void ParticleForceRegistry::add(Particle *particle, ParticleForceGenerator *fg) {
    ParticleForceRegistration newRegistration;
    newRegistration.particle = particle;
    newRegistration.fg = fg;
    registrations.push_back(newRegistration);
}

void ParticleForceRegistry::remove(Particle *particle, ParticleForceGenerator *fg) {
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++) {
        if (i->particle == particle) {
            if (i->fg == fg) {
                registrations.erase(i);
                return;
            }
        }
    }
}

void ParticleForceRegistry::clear() {
    registrations.clear();
}

void ParticleForceRegistry::updateForces(real duration) {
    Registry::iterator i = registrations.begin();
    for (; i != registrations.end(); i++) {
        i->fg->updateForce(i->particle, duration);
    }
}

/*******************************************************************************************************************//**
 *  FORCE GENERATORS
***********************************************************************************************************************/

/** ParticleGravity ***************************************************************************************************/
ParticleGravity::ParticleGravity(const Vector2D &gravity) : gravity(gravity) {}

void ParticleGravity::updateForce(Particle *particle, real duration) {
    if (!particle->hasFiniteMass()) return;
    particle->addForce(gravity * particle->getMass());
}

/** ParticleDrag ******************************************************************************************************/
ParticleDrag::ParticleDrag(real k1, real k2) : k1(k1), k2(k2) {}

void ParticleDrag::updateForce(Particle *particle, real duration) {
    Vector2D force;
    particle->getVelocity(&force);

    // Calculate the total drag coefficient.
    real dragCoeff = force.magnitude();
    dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

    // Calculate the final force and apply it.
    force.normalize();
    force *= -dragCoeff;
    particle->addForce(force);
}

/*******************************************************************************************************************//**
 *  SPRING FORCE GENERATORS
***********************************************************************************************************************/

/** ParticleSpring ****************************************************************************************************/
ParticleSpring::ParticleSpring(Particle *other, real springConstant, real restLength) : other(other),
                                                                                        springConstant(springConstant),
                                                                                        restLength(restLength)
{}

/* ParticleSpring::ParticleSpring(const ParticleSpring &otherSpring) {
 *
 * }
 *
 * ParticleSpring & ParticleSpring::operator=(const ParticleSpring &otherSpring) {
 *     if (this == &otherSpring)
 *         return *this;
 *
 *     delete other;
 *     other = new Particle();
 *     other = otherSpring.other;
 *     springConstant = otherSpring.springConstant;
 *     restLength = otherSpring.restLength;
 *     return *this;
 * }
 */

void ParticleSpring::updateForce(Particle *particle, real duration) {
    // Calculate the vector of the spring
    Vector2D force;
    particle->getPosition(&force);
    force -= other->getPosition();

    // Calculate the magnitude of the force
    real magnitude = force.magnitude();
    magnitude = real_abs(magnitude - restLength);
    magnitude *= springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= -magnitude;
    particle->addForce(force);
}

/** ParticleAnchoredSpring ********************************************************************************************/
ParticleAnchoredSpring::ParticleAnchoredSpring(Vector2D *anchor, real springConstant, real restLength) :
                                                                                anchor(anchor),
                                                                                springConstant(springConstant),
                                                                                restLength(restLength)
{}

void ParticleAnchoredSpring::updateForce(Particle *particle, real duration) {
    // Calculate the vector of the spring
    Vector2D force;
    particle->getPosition(&force);
    force -= *anchor;

    // Calculate the magnitude of the force
    real magnitude = force.magnitude();
    magnitude = (restLength - magnitude) * springConstant;

    // Calculate the final force and apply it
    force.normalize();
    force *= magnitude;
    particle->addForce();
}

/*******************************************************************************************************************//**
 *  EXPERIMENTAL FORCE GENERATORS (From Chapter End Exercises)
***********************************************************************************************************************/

/** ParticleUplift ****************************************************************************************************/
ParticleUplift::ParticleUplift(const Vector2D &uplift, const Vector2D &origin, real range) : uplift(uplift),
                                                                                             origin(origin),
                                                                                             range(range)
{}

void ParticleUplift::updateForce(Particle *particle, real duration) {
    // Determine if the particle is within range of the origin
    Vector2D relativePos = particle->getPosition() - origin;
    if (relativePos.magnitude() > range) return;

    // Particle is in range; apply uplift force
    particle->addForce(uplift * particle->getMass());
}

/** ParticleAirbrake **************************************************************************************************/
ParticleAirbrake::ParticleAirbrake(real k1, real k2, bool isActive) : ParticleDrag(k1, k2), isActive(isActive) {}

void ParticleAirbrake::updateForce(Particle *particle, real duration) {
    /** Do nothing if force generator is inactive */
    if (!isActive) return;

    ParticleDrag::updateForce(particle, duration);
}

void ParticleAirbrake::setActive(bool newActiveState) { isActive = newActiveState; }

void ParticleAirbrake::toggleActive() { isActive = !isActive; }

/** ParticleAttraction ************************************************************************************************/
ParticleAttraction::ParticleAttraction(real magnitude, const Vector2D &origin) : magnitude(magnitude), origin(origin) {}

void ParticleAttraction::updateForce(Particle *particle, real duration) {
    if (!particle->hasFiniteMass()) return;
    Vector2D force;
    particle->getPosition(&force);

    // Calculate direction from particle to force's origin
    force = origin - force;
    force.normalize();
    particle->addForce(force * magnitude * particle->getMass());
}
