#include <emscripten/bind.h>

#include "simulation.hpp"


EMSCRIPTEN_BINDINGS(Gravity) {
    emscripten::class_<Vector2>("Vector2")
            .constructor<double, double>()
            .function("at", emscripten::select_overload<const double&(unsigned int) const>(&Vector2::operator[]));

    emscripten::class_<Body>("Body")
            .function("position", emscripten::select_overload<Vector2() const>(&Body::Position))
            .function("radius", emscripten::select_overload<double() const>(&Body::Radius));

    emscripten::class_<Simulation>("Simulation")
            .constructor()
            .function("addBody", emscripten::select_overload<void(double, double, Vector2, Vector2, bool)>(&Simulation::AddBody))
            .function("update", &Simulation::Update)
            .function("reset", &Simulation::Reset)
            .function("pause", &Simulation::Pause)
            .function("isPaused", &Simulation::IsPaused)
            .function("bodyCount", &Simulation::BodyCount)
            .function("bodies", &Simulation::Bodies)
            .function("getG", emscripten::select_overload<double() const>(&Simulation::G))
            .function("setG", emscripten::select_overload<void(double)>(&Simulation::G))
            .function("setGWithScales", emscripten::select_overload<void(double, double, double)>(&Simulation::G))
            .function("soften", &Simulation::soften)
            .function("getDt", emscripten::select_overload<double() const>(&Simulation::dt))
            .function("setDt", emscripten::select_overload<void(double)>(&Simulation::dt))
            .function("centerOfMass", &Simulation::centerOfMass);

    emscripten::register_vector<Body>("BodyVector");
}

//    void AddBody(double mass, double radius, bool isStatic = false);
//    void AddBody(double mass, double radius, Vector2 position, bool isStatic = false);
//    void AddBody(double mass, double radius, Vector2 position, Vector2 velocity, Vector3 colour, bool isStatic = false);
//    void AddBodies(std::vector<Body> bodies);
//    double Energy() const;
//    Vector2 AngularMomentum() const;
//    Vector2 CalculateTotalForceOnBody(const Body& body);