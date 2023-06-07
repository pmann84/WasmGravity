#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

#include "body.hpp"

static double GCONST = 6.67408e-11;

struct SimulationAxis
{
    double Min;
    double Max;
};

struct SimulationBounds2D
{
    SimulationAxis x_axis;
    SimulationAxis y_axis;
};

class Simulation
{
public:
    Simulation();
    ~Simulation();

    enum IntegrationMethod
    {
        Euler,
        Taylor,
        Leapfrog
    };

    void AddBody(double mass, double radius, bool isStatic = false);
    void AddBody(double mass, double radius, Vector2 position, bool isStatic = false);
    void AddBody(double mass, double radius, Vector2 position, Vector2 velocity, bool isStatic = false);
    void AddBody(double mass, double radius, Vector2 position, Vector2 velocity, Vector3 colour, bool isStatic = false);
    void AddBodies(std::vector<Body> bodies);
    int BodyCount() const;
    void Update();
    void Reset();
    void Pause();
    bool IsPaused();

    double G() const;
    void G(double g);
    void G(double massScale, double timeScale, double lengthScale);

    void soften(bool value);

    double dt() const;
    void dt(double dt);

    Vector2 centerOfMass() const;

    double Energy() const;
    Vector2 AngularMomentum() const;

    const std::vector<Body>& Bodies() const;

private:
    double m_gravConst;
    bool m_bPaused;
    bool m_bDrawVelVectors;
    bool m_soften;
    double m_dt;

    std::vector<Body> m_bodies;
    unsigned int m_nextId;

    SimulationBounds2D m_simBounds;

    void InitSimBounds();
    void SetSimBounds(double xMin, double xMax, double yMin, double yMax);

    Vector2 CalculateTotalForceOnBody(const Body& body, bool soften = false);
    void AddBody(Body& body);
};