#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "particle.h"

#define DISPLAY 500
#define N_PARTICLE 5000
#define FRAME_RATE 60
#define RADIUS 3
#define DELAY 0
#define GRAVITY 1000
#define BOUND_RATIO 0.45
#define THROTTLE 0.5
#define SUBSTEPS 8
#define N_GRID 35

int main() {

    sf::VideoMode window_scale(DISPLAY, DISPLAY);
    sf::RenderWindow window(window_scale, "Verlet Particle Simulation");
    sf::Clock clock;
    sf::Clock fps_clock;
    float last = 0;
    int cycles = 0;

    window.setFramerateLimit(FRAME_RATE);

    float center = DISPLAY / 2;
    float bound = float(DISPLAY) * float(BOUND_RATIO);
    float dt = 1 / (float(FRAME_RATE) * float(SUBSTEPS));

    Particles particles = {DISPLAY / N_GRID};

    while (window.isOpen()) {

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        particles.generate(clock, N_PARTICLE, DELAY);

        for (int substep = 0; substep < SUBSTEPS; substep++) {
            particles.impose_bounds(DISPLAY, RADIUS);
            particles.assign_grid();
            particles.collide_grid(THROTTLE, RADIUS, DISPLAY);
            particles.move(GRAVITY, dt);
        }

        if (cycles == 60) {
            printf(
                "FPS: %3.2f N: %i\n",
                60 / fps_clock.getElapsedTime().asSeconds(),
                int(particles.x_pos.size())
            );
            fps_clock.restart();
            cycles = 0;
        }
        

        cycles += 1;
        particles.render(window, RADIUS);
    }
}