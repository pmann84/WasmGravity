#include "simulation.hpp"

Simulation::Simulation() : m_nextId(0), m_dt(0.001)
{
    InitSimBounds();
}

Simulation::~Simulation()
{
}

void Simulation::InitSimBounds()
{
    SetSimBounds(-3.0, 3.0, -3.0, 3.0);
}

void Simulation::SetSimBounds(double xMin, double xMax, double yMin, double yMax)
{
    m_simBounds.x_axis.Min = xMin;
    m_simBounds.x_axis.Max = xMax;
    m_simBounds.y_axis.Min = yMin;
    m_simBounds.y_axis.Max = yMax;
}

void Simulation::AddBody(Body& body)
{
    m_bodies.push_back(body);
}

void Simulation::AddBody(double mass, double radius, bool isStatic)
{
    Body newBody(m_nextId++, mass, radius);
    newBody.Static(isStatic);
    AddBody(newBody);
}

void Simulation::AddBody(double mass, double radius, Vector2 position, bool isStatic)
{
    Body newBody(m_nextId++, mass, radius);
    newBody.Position(position);
    newBody.Static(isStatic);
    AddBody(newBody);
}

void Simulation::AddBody(double mass, double radius, Vector2 position, Vector2 velocity, bool isStatic)
{
    Body newBody(m_nextId++, mass, radius);
    newBody.Position(position);
    newBody.Velocity(velocity);
    newBody.Static(isStatic);
    AddBody(newBody);
}

void Simulation::AddBody(double mass, double radius, Vector2 position, Vector2 velocity, Vector3 colour, bool isStatic)
{
    Body newBody(m_nextId++, mass, radius);
    newBody.Position(position);
    newBody.Velocity(velocity);
    newBody.Colour(colour);
    newBody.Static(isStatic);
    AddBody(newBody);
}

void Simulation::AddBodies(std::vector<Body> bodies)
{
    for (auto& body : bodies)
    {
        AddBody(body);
    }
}

int Simulation::BodyCount() const
{
    return m_bodies.size();
}

Vector2 Simulation::CalculateTotalForceOnBody(const Body& body, bool soften)
{
    Vector2 force_agg;
    for (auto forceFromBody : m_bodies)
    {
        // Only add force contributions of other bodies, not itself
        if (body.Id() != forceFromBody.Id())
        {
            // Calculate the contribution of the force between the two bodies
            force_agg += body.ForceExertedBy(forceFromBody, G(), soften);
        }
    }
    return force_agg;
}

void Simulation::Update()
{
    // Don't do anything if we are paused
    if (m_bPaused) return;

    auto intMethod = IntegrationMethod::Leapfrog;

    Vector2 force_agg, new_force_agg;
    if (m_bodies.size() > 0)
    {
        // Loop over each body to calculate new position
        for (auto& body : m_bodies)
        {
            // Only calculate new position if body is not statics
            if (!body.Static())
            {
                Vector2 new_vel, new_pos;
                Vector2 temp_vel, temp_pos;

                double dt = m_dt;

                switch (intMethod)
                {
                    case IntegrationMethod::Euler:
                        // Euler
                        // v_n+1 = v_n + sum(F)*dt
                        // p_n+1 = p_n + v_n+1 *dt
                        force_agg = CalculateTotalForceOnBody(body, m_soften);
                        new_vel = body.Velocity() + force_agg*dt;
                        new_pos = body.Position() + new_vel*dt;
                        body.Acceleration(force_agg);
                        body.Velocity(new_vel);
                        body.Position(new_pos);
                        break;
                    case IntegrationMethod::Taylor:
                        // Taylor Series
                        // v_n+1 = v_n + sum(F)*dt
                        // p_n+1 = p_n + v_n+1 *dt + 0.5*sum(F)*dt*dt
                        force_agg = CalculateTotalForceOnBody(body, m_soften);
                        new_vel = body.Velocity() + force_agg*dt;
                        new_pos = body.Position() + new_vel*dt + 0.5*force_agg*dt*dt;
                        body.Acceleration(force_agg);
                        body.Velocity(new_vel);
                        body.Position(new_pos);
                        break;
                    case IntegrationMethod::Leapfrog:
                        // Leapfrog
                        // r_n+0.5 = r_n + 0.5*dt*v_n
                        // v_n+1 = v_n + dt*a(r_n+0.5)
                        // r_n+1 = r_n+0.5 + 0.5*dt*v_n+1
                        temp_pos = body.Position() + dt*0.5*body.Velocity();
                        body.Position(temp_pos);
                        force_agg = CalculateTotalForceOnBody(body, m_soften);
                        new_vel = body.Velocity() + dt*force_agg;
                        new_pos = temp_pos + 0.5*dt*new_vel;
                        body.Velocity(new_vel);
                        body.Position(new_pos);
                        break;
                        // Improved Euler
                        // a_n = sum(F(p_n))
                        // p_temp = p_n + v_n * dt
                        // a_temp = sum(F(p_temp))
                        // v_n+1 = v_n + 0.5 * (a_n + a_temp) * dt
                        // p_n+1 = p_n + 0.5 * (v_n+1 + v_n) * dt
                }
            }
        }
    }
}

void Simulation::Reset()
{
    for (auto& body : m_bodies)
    {
        body.Position(body.InitialPosition());
        body.Velocity(body.InitialVelocity());
    }
}

void Simulation::Pause()
{
    m_bPaused = !m_bPaused;
}

bool Simulation::IsPaused()
{
    return m_bPaused;
}

double Simulation::G() const
{
    return m_gravConst;
}

void Simulation::G(double g)
{
    m_gravConst = g;
}

void Simulation::G(double massScale, double timeScale, double lengthScale)
{
    m_gravConst = (GCONST * massScale * timeScale * timeScale) / (lengthScale * lengthScale * lengthScale);
}

double Simulation::Energy() const
{
    // E = 0.5 * sum{i=1..N}(m_i v_i^2) + sum{i=1..N}(sum{j!=i}(Gm_im_j/|r_i - r_j|))
    double energy = 0.0;
    for (auto& body : m_bodies)
    {
        energy += 0.5*body.Mass()*body.Velocity().NormSquared(); // Kinetic energy
        // Get all bodies with
        for (auto& potentialBody : m_bodies)
        {
            if (body.Id() != potentialBody.Id())
            {
                energy -= body.GravitationalPotential(potentialBody, m_gravConst);
            }
        }
    }
    return energy;
}

Vector2 Simulation::AngularMomentum() const
{
    // J = sum{i=1..N}(r_i cross m_i v_i)
    return Vector2();
}

const std::vector<Body>& Simulation::Bodies() const
{
    return m_bodies;
}

void Simulation::soften(bool value)
{
    m_soften = value;
}

void Simulation::dt(double dt)
{
    m_dt = dt;
}

double Simulation::dt() const
{
    return m_dt;
}

Vector2 Simulation::centerOfMass() const
{
    // Get sum of position of all bodies * its mass / total mass
    Vector2 CoM;
    double totalMass = 0;
    for (auto& body : m_bodies)
    {
        totalMass += body.Mass();
        CoM += body.Position() * body.Mass();
    }
    return CoM * (1.0 / totalMass);
}

void GetTransformedPositions(double coord_x, double coord_y,
                             double xmin1, double xmax1, double ymin1, double ymax1,
                             double xmin2, double xmax2, double ymin2, double ymax2,
                             double& new_coord_x, double& new_coord_y)
{
    // Get percentage of x & y in sim space
    double x_percent = (coord_x - xmin1) / (xmax1 - xmin1);
    double y_percent = (coord_y - ymin1) / (ymax1 - ymin1);

    // Get values of same percent in screen space
    new_coord_x = x_percent * abs(xmax2 - xmin2) + xmin2;
    new_coord_y = y_percent * abs(ymax2 - ymin2) + ymin2;
}