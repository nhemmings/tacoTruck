/*
 * Implementation of the particle class.
 *
 * Author: Nathan Hemmings
 *
 * Date created: 07/06/2015
 *
 */
#include <assert.h>
#include <cmath>
#include "particle.hpp"

using namespace tacoTruck;

Particle::Particle() : inverseMass(1.0f),
                       damping(0.999f),
                       position(0, 0),
                       velocity(0, 0),
                       acceleration(0, 0),
                       forceAccum(0,0)
{}

void Particle::integrate(real duration) {
    // Don't integrate things with infinite mass
    if (inverseMass <= 0.0f) return;
    assert(duration > 0.0f);

    // Update linear position
    position.addScaledVector(velocity, duration);

    // Account for acceleration due to forces
    Vector2D resultingAcc = acceleration;
    resultingAcc.addScaledVector(forceAccum, inverseMass);

    // Update velocity
    velocity.addScaledVector(resultingAcc, duration);

    // Impose drag
    velocity *= std::pow(damping, duration);

    // Clear the accumulated forces
    clearAccumulator();
}

void Particle::setMass(const real mass) {
    assert (mass != 0);
    inverseMass = ((real)1.0)/mass;
}

real Particle::getMass() const {
    if (inverseMass == 0) {
        return REAL_MAX;
    } else {
        return ((real)1.0)/inverseMass;
    }
}

void Particle::setInverseMass(const real inverseMass) {
    Particle::inverseMass = inverseMass;
}

real Particle::getInverseMass() const {
    return inverseMass;
}

void Particle::setDamping(const real damping)
{
    Particle::damping = damping;
}

real Particle::getDamping() const
{
    return damping;
}

void Particle::setPosition(const Vector2D& position) {
    Particle::position = position;
}

void Particle::setPosition(const real x, const real y) {
    position.x = x;
    position.y = y;
}

void Particle::getPosition(Vector2D* position) const {
    *position = Particle::position;
}

Vector2D Particle::getPosition() const {
    return position;
}

void Particle::setVelocity(const Vector2D& velocity) {
    Particle::velocity = velocity;
}

void Particle::setVelocity(const real x, const real y) {
    velocity.x = x;
    velocity.y = y;
}

void Particle::getVelocity(Vector2D* velocity) const {
    *velocity = Particle::velocity;
}

Vector2D Particle::getVelocity() const {
    return velocity;
}

void Particle::setAcceleration(const Vector2D& acceleration) {
    Particle::acceleration = acceleration;
}

void Particle::setAcceleration(const real x, const real y) {
    acceleration.x = x;
    acceleration.y = y;
}

void Particle::getAcceleration(Vector2D* acceleration) const {
    *acceleration = Particle::acceleration;
}

Vector2D Particle::getAcceleration() const {
    return acceleration;
}

bool Particle::hasFiniteMass() const {
    return inverseMass >= 0.0f;
}

void Particle::clearAccumulator() {
    forceAccum.clear();
}

void Particle::addForce(const Vector2D& force) {
    forceAccum += force;
}
