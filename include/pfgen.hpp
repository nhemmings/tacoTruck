#ifndef PHYSICS_PFGEN_HPP_
#define PHYSICS_PFGEN_HPP_

#include <vector>
#include "Vector2D.hpp"
#include "particle.hpp"

namespace tacoTruck {
/**
 *  A force generator can be asked to add a force to one or more particles.
 */
class ParticleForceGenerator {
public:
    /**
     *  Overload this in implementations of the interface to calculate and update the force applied to
     *  the given particle.
     *
     *  @param particle the particle to apply a force to
     *  @param duration the amount of time (in seconds) to simulate (for forces affected by time)
     */
     virtual void updateForce(Particle *particle, real duration) = 0;
     virtual ~ParticleForceGenerator() {}
};

/** Holds all the force generators and the particles that they apply to. */
class ParticleForceRegistry {
protected:
    /** Keeps track of one force generator and the particle it applies to. */
    struct ParticleForceRegistration {
        Particle *particle;
        ParticleForceGenerator *fg;
    };

    /** Holds the list of registrations. */
    typedef std::vector<ParticleForceRegistration> Registry;
    Registry registrations;

public:
    /** Registers the given force generator to apply to the given particle. */
    void add(Particle *particle, ParticleForceGenerator *fg);

    /** Removes the given registered pair from the registry.
     *  If the pair is not registered, this method will have no effect.
     */
    void remove(Particle *particle, ParticleForceGenerator *fg);

    /** Clears all registrations from the registry.
     *  This will not delete the particles or the force generators themselves,
     *  just the records of their connection.
     */
    void clear();

    /** Calls all the force generators to update the forces of their corresponding particles. */
    void updateForces(real duration);
};

/*******************************************************************************************************************//**
 *  FORCE GENERATORS
***********************************************************************************************************************/

/** A force generator that applies a gravitational force. One instance can be used for multiple particles. */
class ParticleGravity : public ParticleForceGenerator {
    /** Holds the acceleration due to gravity. */
    Vector2D gravity;

public:
    /** Creates the generator with the given acceleration. */
    ParticleGravity(const Vector2D &gravity);

    /** Applies the gravitational force to the given particle. */
    virtual void updateForce(Particle *particle, real duration);
};

/** A force generator that applies a drag force. One instance can be used for multiple particles. */
class ParticleDrag : public ParticleForceGenerator {
    real k1;    /**< Holds the velocity drag coefficient. */
    real k2;    /**< Holds the velocity squared drag coefficient. */

public:
    /** Creates the generator with the given coefficients. */
    ParticleDrag(real k1, real k2);

    /** Applies the drag force to the given particle. */
    virtual void updateForce(Particle *particle, real duration);
};

/** EXPERIMENTAL */
class ParticleUplift : public ParticleForceGenerator {
    /** Holds the uplift force */
    Vector2D uplift;
    Vector2D origin;
    real range;

public:
    /** Creates the generator with the given force, origin, and effect range */
    ParticleUplift(const Vector2D &uplift, const Vector2D &origin, real range);

    /** Applies the uplift force to the given particle */
    virtual void updateForce(Particle *particle, real duration);
};

}   // namespace tacoTruck
#endif // PHYSICS_PFGEN_HPP_
