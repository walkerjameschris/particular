#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "src/particle.hpp"
#include "src/hud.hpp"

int main() {

    const int min_particle = 1;
    const int max_particle = 5000;
    const int particle_step = 10;
    const int display_x = 1280;
    const int display_y = 720;
    const int frame_rate = 60;
    const int radius = 6;
    const int substeps = 4;
    const int ppc = 10;
    const float width = sqrt(ppc) * radius;
    const int n_grid_x = ceil(display_x / width);
    const int n_grid_y = ceil(display_y / width);
    const float gravity = 1000;
    const float max_shift = 0.20;
    const float dt = 1 / float(frame_rate * substeps);

    int n_particle = max_particle / 2;
    float gravity_x = 0.0;
    float gravity_y = gravity;
    bool center = false;
    bool explode = false;

    Particles particles = {
        n_particle,
        n_grid_x,
        n_grid_y,
        display_x,
        display_y,
        width,
        radius,
        max_shift,
        gravity
    };

    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Verlet Simulation");
    window.setFramerateLimit(int(frame_rate));
    sf::Clock clock;
    HUD hud;

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            center = true;
            explode = false;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            center = false;
            explode = true;
        } else {
            center = false;
            explode = false;
        }

        bool minus = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        bool add = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        bool info = sf::Keyboard::isKeyPressed(sf::Keyboard::I);
        bool velocity = sf::Keyboard::isKeyPressed(sf::Keyboard::V);

        if (minus && n_particle >= (min_particle + particle_step)) {
            n_particle -= particle_step;
        } else if (add && n_particle <= (max_particle - particle_step)) {
            n_particle += particle_step;
        }

        for (int i = 0; i < substeps; i++) {
            particles.generate(n_particle, display_x);
            particles.impose_bounds();
            particles.assign_grid();
            particles.collide_grid();
            particles.move(gravity_x, gravity_y, dt, center, explode);
        }

        window.clear();

        hud.render(
            window,
            clock,
            gravity_x / gravity,
            gravity_y / gravity,
            particles.x_pos.size(),
            n_particle,
            n_grid_x,
            n_grid_y,
            info,
            center,
            explode
        );

        particles.render(window, velocity);
        window.display();
    }
}
