#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace sf;
using namespace std;

const float G = 6.67430e-5f;      // Gravitational constant (scaled for visualization)
const float timeStep = 0.1f;      // Time step for simulation
const Vector2f windowSize(1600, 900);

struct Planet {
    CircleShape shape;
    Vector2f velocity;
    float mass;

    Planet(float radius, float mass, Vector2f position, Vector2f initialVelocity, Color color) {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius);
        shape.setPosition(position);
        shape.setFillColor(color);
        this->mass = mass;
        this->velocity = initialVelocity;
    }
};

class GravitySimulation {
private:
    RenderWindow window;
    vector<Planet> planets;
    CircleShape sun;

public:
    GravitySimulation() : window(VideoMode(windowSize.x, windowSize.y), "Enhanced Gravity Simulation") {
        window.setFramerateLimit(60);
        setupPlanets();
    }

    void setupPlanets() {
        // Sun
        sun.setRadius(40.f);
        sun.setOrigin(40.f, 40.f);
        sun.setPosition(windowSize.x / 2, windowSize.y / 2);
        sun.setFillColor(Color::Yellow);

        // Planets
        planets.emplace_back(10.f, 1000.f, Vector2f(windowSize.x / 2 + 150, windowSize.y / 2), Vector2f(0, -0.9f), Color::Blue);
        planets.emplace_back(12.f, 1500.f, Vector2f(windowSize.x / 2 + 250, windowSize.y / 2), Vector2f(0, -0.7f), Color::Red);
        planets.emplace_back(8.f, 800.f, Vector2f(windowSize.x / 2 + 350, windowSize.y / 2), Vector2f(0, -0.5f), Color::Green);
    }

    Vector2f calculateGravity(const Planet& planet) {
        Vector2f sunPosition = sun.getPosition();
        Vector2f planetPosition = planet.shape.getPosition();
        Vector2f direction = sunPosition - planetPosition;

        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance < sun.getRadius()) distance = sun.getRadius();

        float force = (G * planet.mass * 1000000.f) / (distance * distance);
        Vector2f normalizedDirection = direction / distance;

        return normalizedDirection * force;
    }

    void update() {
        for (auto& planet : planets) {
            Vector2f acceleration = calculateGravity(planet) / planet.mass;
            planet.velocity += acceleration * timeStep;
            planet.shape.move(planet.velocity * timeStep);
        }
    }

    void renderSpaceTimeGrid() {
        const int gridSize = 50;
        VertexArray lines(Lines);

        for (int x = 0; x < windowSize.x; x += gridSize) {
            lines.append(Vertex(Vector2f(x, 0), Color(100, 100, 100)));
            lines.append(Vertex(Vector2f(x, windowSize.y), Color(100, 100, 100)));
        }

        for (int y = 0; y < windowSize.y; y += gridSize) {
            lines.append(Vertex(Vector2f(0, y), Color(100, 100, 100)));
            lines.append(Vertex(Vector2f(windowSize.x, y), Color(100, 100, 100)));
        }

        window.draw(lines);
    }

    void drawCurvedSpaceTime() {
        const int radius = 150;
        CircleShape spacetimeEffect(radius);
        spacetimeEffect.setFillColor(Color(0, 0, 0, 0));
        spacetimeEffect.setOutlineColor(Color(255, 255, 0, 100));
        spacetimeEffect.setOutlineThickness(1.0f);
        spacetimeEffect.setOrigin(radius, radius);
        spacetimeEffect.setPosition(sun.getPosition());

        for (float scale = 1.0f; scale > 0.1f; scale -= 0.05f) {
            spacetimeEffect.setScale(scale, scale);
            window.draw(spacetimeEffect);
        }
    }

    void run() {
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            window.clear(Color::Black);

            renderSpaceTimeGrid();
            drawCurvedSpaceTime();

            window.draw(sun);
            update();

            for (auto& planet : planets) {
                window.draw(planet.shape);
            }

            window.display();
        }
    }
};

int main() {
    GravitySimulation simulation;
    simulation.run();
    return 0;
}