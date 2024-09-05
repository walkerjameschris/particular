#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "src/particle.hpp"
#include "src/hud.hpp"

int main() {

    // Constants
    const int min_particle = 1;
    const int max_particle = 5000;
    const int particle_step = 10;
    const int display_x = 1400;
    const int display_y = 600;
    const int frame_rate = 60;
    const int radius = 4;
    const int substeps = 3;
    const int ppc = 8;
    const float gravity = 1000;
    const float max_shift = 0.2;

    // Dynamic parameters
    int n_particle = 5000;
    float gravity_x = 0.0;
    float gravity_y = gravity;
    float dt = 1 / float(frame_rate * substeps);
    float width = sqrt(ppc) * radius;
    int n_grid_x = ceil(display_x / width);
    int n_grid_y = ceil(display_y / width);
    int wave_adj = 0;
    int wave_ind = 0;
    int wave_max = display_x * 0.2;

    // Particle container
    Particles particles = {
        n_particle,
        n_grid_x,
        n_grid_y,
        display_x,
        display_y,
        width,
        radius,
        max_shift
    };

    // Initialization
    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Verlet Simulation");
    window.setFramerateLimit(int(frame_rate));
    sf::Clock clock;
    HUD hud;

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

        // Other user presses
        bool minus = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        bool add = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        bool space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

        // Adjust particle generation
        if (minus && n_particle >= (min_particle + particle_step)) {
            n_particle -= particle_step;
        } else if (add && n_particle <= (max_particle - particle_step)) {
            n_particle += particle_step;
        }

        // Substep simulation solver
        for (int i = 0; i < substeps; i++) {
            particles.generate(n_particle, display_x);
            particles.impose_bounds();
            particles.assign_grid();
            particles.collide_grid();
            particles.move(gravity_x, gravity_y, dt);
        }

        // Clear display elements
        window.clear();

        // Handle HUD press
        hud.try_toggle(sf::Keyboard::isKeyPressed(sf::Keyboard::I));

        // Handle HUD
        hud.render(
            window,
            clock,
            gravity_x / gravity,
            gravity_y / gravity,
            particles.x_pos.size(),
            n_particle,
            n_grid_x,
            n_grid_y
        );

        // Render and display
        particles.render(window, sf::Keyboard::isKeyPressed(sf::Keyboard::V));
        window.display();
    }
}
