#include "body.hpp"

Body::Body(unsigned int id) : m_iBodyId(id), m_dMass(1.0), m_dRadius(1.0)
{
}

Body::Body(unsigned int id, double mass, double radius) : m_iBodyId(id), m_dMass(mass), m_dRadius(radius)
{
}

Body::Body(const Body& body) :
        m_iBodyId(body.Id()),
        m_dMass(body.Mass()),
        m_dRadius(body.Radius()),
        m_vPosition(body.Position()),
        m_vInitialPosition(body.Position()),
        m_vVelocity(body.Velocity()),
        m_vInitialVelocity(body.Velocity()),
        m_vColour(body.Colour()),
        m_bStatic(body.Static())
{
}

Body::~Body()
{
}

Vector2 Body::Position() const
{
    return m_vPosition;
}

void Body::Position(const Vector2& rPos)
{
    m_vPosition = rPos;
}

Vector2 Body::InitialPosition() const
{
    return m_vInitialPosition;
}

void Body::InitialPosition(const Vector2& rPos)
{
    m_vInitialPosition = rPos;
}

Vector2 Body::Velocity() const
{
    return m_vVelocity;
}

void Body::Velocity(const Vector2& rVel)
{
    m_vVelocity = rVel;
}

Vector2 Body::InitialVelocity() const
{
    return m_vInitialVelocity;
}

void Body::InitialVelocity(const Vector2& rVel)
{
    m_vInitialVelocity = rVel;
}

Vector2 Body::Acceleration() const
{
    return m_vAcceleration;
}

void Body::Acceleration(const Vector2& rAcc)
{
    m_vAcceleration = rAcc;
}

Vector3 Body::Colour() const
{
    return m_vColour;
}

void Body::Colour(const Vector3& rColour)
{
    m_vColour = rColour;
}

Vector2 Body::DistVectToBody(const Body& body) const
{
    return body.Position() - Position();
}

double Body::GravitationalPotential(const Body& body, double G) const
{
    return G*body.Mass()*Mass() / DistVectToBody(body).Norm();
}

double Body::GravitationalForce(const Body& body, double G) const
{
    auto normalisedDistToBody = DistVectToBody(body).Norm();
    return (G*body.Mass()*Mass()) / (normalisedDistToBody*normalisedDistToBody);
}

double Body::GravitationalForce(Vector2& distBetweenBodies, float bodyMass, double G) const
{
    auto normalisedDistToBody = distBetweenBodies.Norm();
    return (G*bodyMass*Mass()) / (normalisedDistToBody*normalisedDistToBody);
}

double Body::SoftenedGravitationalForce(const Body& body, double G, double softening) const
{
    auto normalisedDistToBody = DistVectToBody(body).Norm();
    double denom = std::pow((normalisedDistToBody*normalisedDistToBody - softening*softening), 1.5);
    return (G*body.Mass()*Mass()) / denom;
}

double Body::SoftenedGravitationalForce(Vector2& distBetweenBodies, float bodyMass, double G, double softening) const
{
    auto normalisedDistToBody = distBetweenBodies.Norm();
    double denom = std::pow((normalisedDistToBody*normalisedDistToBody - softening*softening), 1.5);
    return (G*bodyMass*Mass()) / denom;
}

Vector2 Body::ForceExertedBy(const Body& body, double G) const
{
    // MINIMISE CALL TO DistVectToBody - the calculation of distance takes aaaaages, so reduce this
    auto suppliedBodyPos = body.Position();
    auto currentBodyPos = Position();
    auto distVector = suppliedBodyPos - currentBodyPos; // THIS LINE IS SLOOOOOOOOW
    distVector.Normalise();
    auto forceAdd = SoftenedGravitationalForce(distVector, body.Mass(), G, 0.01)*(1.0/Mass())*distVector; // THIS LINE IS SLOOOOOOOOOW
    //auto forceAdd = GravitationalForce(distVector, body.Mass(), G)*(1.0 / Mass())*distVector; // THIS LINE IS SLOOOOOOOOOW
    return forceAdd;
}