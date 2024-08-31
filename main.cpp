#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "src/particle.hpp"

#define DISPLAY 500
#define N_PARTICLE 5000
#define FRAME_RATE 60
#define RADIUS 2
#define SUBSTEPS 8
#define N_GRID 35
#define GRAVITY 1000
#define CHECK 60

int main() {

    // Initialization
    sf::VideoMode window_scale(DISPLAY, DISPLAY);
    sf::RenderWindow window(window_scale, "Verlet Particle Simulation");
    sf::Clock clock;

    // Window adjustments
    unsigned int rescale = DISPLAY * 1.75;
    window.setFramerateLimit(int(FRAME_RATE));
    window.setSize(sf::Vector2(rescale, rescale));

    // Dynamic parameters
    float gravity_x = 0.0;
    float gravity_y = GRAVITY;
    float dt = 1 / (float(FRAME_RATE) * float(SUBSTEPS));
    int cycles = 0;

    // Particle container
    Particles particles = {
        N_PARTICLE,
        N_GRID,
        DISPLAY,
        RADIUS
    };

    // Main simulation
    while (window.isOpen()) {

        // Handle exits
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // User adjusted gravity
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            gravity_x = 0;
            gravity_y = -GRAVITY;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            gravity_x = -GRAVITY;
            gravity_y = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            gravity_x = GRAVITY;
            gravity_y = 0;
        } else {
            gravity_x = 0;
            gravity_y = GRAVITY;
        }

        // New particle generation
        particles.generate();

        // Substep simulation solver
        for (int i = 0; i < SUBSTEPS; i++) {
            particles.impose_bounds();
            particles.assign_grid();
            particles.collide_grid();
            particles.move(gravity_x, gravity_y, dt);
        }

        // Prints FPS counter to terminal
        if (cycles == CHECK) {
            float fps = float(CHECK) / clock.getElapsedTime().asSeconds();
            int n = int(particles.x_pos.size());
            printf("FPS: %3.1f Particles: %i\n", fps, n);
            clock.restart();
            cycles = 0;
        }

        cycles += 1;
        particles.render(window);
    }
}