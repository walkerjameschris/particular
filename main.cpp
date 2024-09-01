#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "src/particle.hpp"

int main() {

    // Constants
    const float display = 500;
    const float n_particle = 2000;
    const float frame_rate = 60;
    const float radius = 3;
    const float substeps = 12;
    const float n_grid = 24;
    const float gravity = 1000;

    // Initialization
    sf::VideoMode window_scale(display, display);
    sf::RenderWindow window(window_scale, "Verlet Particle Simulation");
    sf::Clock clock;

    // Window adjustments
    unsigned int rescale = display * 1.75;
    window.setFramerateLimit(int(frame_rate));
    window.setSize(sf::Vector2(rescale, rescale));

    // Dynamic parameters
    float gravity_x = 0.0;
    float gravity_y = gravity;
    float dt = 1 / (frame_rate * substeps);
    int cycles = 0;

    // Particle container
    Particles particles = {
        n_particle,
        n_grid,
        display,
        radius
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
            gravity_y = -gravity;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            gravity_x = -gravity;
            gravity_y = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            gravity_x = gravity;
            gravity_y = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            gravity_x = 0;
            gravity_y = 0;
        } else {
            gravity_x = 0;
            gravity_y = gravity;
        }
        // New particle generation
        particles.generate();

        // Substep simulation solver
        for (int i = 0; i < substeps; i++) {
            particles.impose_bounds();
            particles.assign_grid();
            particles.collide_grid();
            particles.move(gravity_x, gravity_y, dt);
        }

        // Prints FPS counter to terminal
        if (cycles > frame_rate) {
            float fps = frame_rate / clock.getElapsedTime().asSeconds();
            int n = int(particles.x_pos.size());
            printf("FPS: %3.1f Particles: %i\n", fps, n);
            clock.restart();
            cycles = 0;
        }

        // Render and count
        particles.render(window);
        cycles += 1;
    }
}