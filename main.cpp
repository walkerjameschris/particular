#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "src/particle.hpp"
#include "src/hud.hpp"

int main() {

    // Constants
    const float display = 800;
    const float n_particle = 2500;
    const float frame_rate = 60;
    const float radius = 4;
    const float substeps = 6;
    const float ppc = 100;
    const float gravity = 1000;
    const float max_shift = 0.2;

    // SFML initialization
    sf::VideoMode window_scale(display, display);
    sf::RenderWindow window(window_scale, "Verlet Simulation");
    window.setFramerateLimit(int(frame_rate));
    sf::Clock clock;

    // HUD initialization
    HUD hud;
    hud.init();

    // Dynamic parameters
    float gravity_x = 0.0;
    float gravity_y = gravity;
    float dt = 1 / (frame_rate * substeps);
    float n_grid = floor(3 * sqrt(pow(display / radius, 2) / ppc));

    // Particle container
    Particles particles = {
        n_particle,
        n_grid,
        display,
        radius,
        max_shift
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

        // Clear display elements
        window.clear();

        // Handle HUD
        hud.render(
            window,
            clock,
            sf::Keyboard::isKeyPressed(sf::Keyboard::I),
            gravity_x / gravity,
            gravity_y / gravity,
            particles.x_pos.size(),
            n_particle,
            n_grid
        );

        // Render and display
        particles.render(window);
        window.display();
    }
}
