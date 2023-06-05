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
            .function("bodies", &Simulation::Bodies);

    emscripten::register_vector<Body>("BodyVector");
}

//    void AddBody(double mass, double radius, bool isStatic = false);
//    void AddBody(double mass, double radius, Vector2 position, bool isStatic = false);
//    void AddBody(double mass, double radius, Vector2 position, Vector2 velocity, Vector3 colour, bool isStatic = false);
//    void AddBodies(std::vector<Body> bodies);
//    double G() const;
//    void G(double g);
//    void G(double massScale, double timeScale, double lengthScale);
//    double Energy() const;
//    Vector2 AngularMomentum() const;
//    Vector2 CalculateTotalForceOnBody(const Body& body);
//    std::vector<Body> Bodies() const;