#pragma once

#include "vector.hpp"

class Body
{
public:
    Body(unsigned int id);
    Body(unsigned int id, double mass, double radius);
    Body(const Body& body);
    ~Body();

    Vector2 Position() const;
    void Position(const Vector2& rPos);

    Vector2 InitialPosition() const;
    void InitialPosition(const Vector2& rPos);

    Vector2 Velocity() const;
    void Velocity(const Vector2& rVel);

    Vector2 InitialVelocity() const;
    void InitialVelocity(const Vector2& rVel);

    Vector2 Acceleration() const;
    void Acceleration(const Vector2& rAcc);

    Vector3 Colour() const;
    void Colour(const Vector3& rColour);

    unsigned int Id() const { return m_iBodyId; };

    double Mass() const { return m_dMass; };
    void Mass(double m) { m_dMass = m; };

    double Radius() const { return m_dRadius; };
    void Radius(double r) { m_dRadius = r; };

    bool Static() const { return m_bStatic; };
    void Static(bool s) { m_bStatic = s; };

    Vector2 DistVectToBody(const Body& body) const;

    double GravitationalPotential(const Body& body, double G) const;
    double GravitationalForce(const Body& body, double G) const;
    double GravitationalForce(Vector2& distBetweenBodies, float bodyMass, double G) const;
    double SoftenedGravitationalForce(const Body& body, double G, double softening) const;
    double SoftenedGravitationalForce(Vector2& distBetweenBodies, float bodyMass, double G, double softening) const;
    Vector2 ForceExertedBy(const Body& body, double G) const;

    void operator=(const Body& body)
    {
        m_dMass = body.Mass();
        m_dRadius = body.Radius();
        m_vPosition = body.Position();
        m_vVelocity = body.Velocity();
        m_vAcceleration = body.Acceleration();
        m_bStatic = body.Static();
    }

private:
    const unsigned int m_iBodyId;
    double m_dMass;
    double m_dRadius;
    Vector2 m_vPosition;
    Vector2 m_vInitialPosition;
    Vector2 m_vVelocity;
    Vector2 m_vInitialVelocity;
    Vector2 m_vAcceleration;
    Vector3 m_vColour;
    bool m_bStatic;
};