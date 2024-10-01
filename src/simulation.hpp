#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "utilities.hpp"
#include <vector>
#include <string>
#include <cmath>

struct Simulation {

    Particles particles;
    std::unordered_map<int, std::vector<int>> grid;
    sf::Vector2f gravity_system;
    
    int display_x;
    int display_y;
    int n_grid_x;
    int n_grid_y;
    float delta;

    int substeps = 5;
    float max_shift = 0.2;
    float force = 1000;
    float width = 18;

    Simulation(int x, int y, int fps, bool use_grid, std::string path) {

        particles = load_spec(path);

        display_x = x;
        display_y = y;
        n_grid_x = ceil(float(display_x) / width);
        n_grid_y = ceil(float(display_y) / width);
        delta = 1 / float(fps * substeps);

        if (!use_grid) {
            n_grid_x = 1;
            n_grid_y = 1;
            width = std::max(display_x, display_y);
        }
    }

    void impose_bounds() {
        for (Particle& i : particles) {
            i.position.x = clamp(i.position.x, 0, display_x - i.radius);
            i.position.y = clamp(i.position.y, 0, display_y - i.radius);
        }
    }

    void assign_grid() {

        grid.clear();
        int index = 0;

        for (Particle& i : particles) {
            int id = hash_id(i.position.x, i.position.y, width);
            grid[id].emplace_back(index);
            index += 1;
        }
    }

    void collide_inner(int inner_id, int outer_id) {
        // High performance Verlet integrator which uses
        // a subspace grid to transform search space from
        // O(N^2) to approximately O(N) for particle
        // collision. This detects whether two particles
        // are within some distance and nudges them apart
        // along the axis of collision to solve particle
        // interaction.

        std::vector<int>& inner = grid[inner_id];
        std::vector<int>& outer = grid[outer_id];
        bool same_cell = inner_id == outer_id;

        int start = 0;

        if (inner.size() == 0 || outer.size() == 0) {
            return;
        }

        for (int a = 0; a < inner.size(); a++) {

            if (same_cell) {
                start = a + 1;
            }

            for (int b = start; b < outer.size(); b++) {

                int i_id = inner[a];
                int j_id = outer[b];

                Particle& i = particles[i_id];
                Particle& j = particles[j_id];
                
                bool linked = i.linked == j_id || j.linked == i_id;

                sf::Vector2f change = i.position - j.position;
                float distance = sqrt(pow(change.x, 2) + pow(change.y, 2));
                float tolerance = i.radius + j.radius;

                float scalar = 0.5 * (distance - tolerance);
                sf::Vector2f divisor = scalar * change / distance;

                if (distance < tolerance) {
                    divisor.x = clamp(divisor.x, -max_shift, max_shift);
                    divisor.y = clamp(divisor.y, -max_shift, max_shift);
                }

                if (distance < tolerance || linked) {

                    if (!i.fixed) {
                        i.position -= divisor;
                    }

                    if (!j.fixed) {
                        j.position += divisor;
                    }
                }
            }
        }
    }

    void collide() {
        // Iterates through the simulation space grid. This
        // cycles through all grid cells and detects collision
        // between the current cell and all neighboring cells.

        for (int i = 0; i < n_grid_x; i++) {
            for (int j = 0; j < n_grid_y; j++) {
                for (int a = i - 1; a < (i + 1); a++) {
                    for (int b = j - 1; b < (j + 1); b++) {
                        collide_inner(hash(i, j), hash(a, b));
                    }
                }
            }
        }
    }

    void adjust_gravity(bool up, bool left, bool right, bool zero) {

        gravity_system = {0, force};
        
        if (zero) {
            gravity_system = {0, 0};
        } else if (up) {
            gravity_system = {0, -force};
        } else if (left) {
            gravity_system = {-force, 0};
        } else if (right) {
            gravity_system = {force, 0};
        }
    }
    
    void render(sf::RenderWindow& window) {

        window.clear();

        sf::CircleShape circle;

        for (Particle& i : particles) {
            circle.setPointCount(32);
            circle.setRadius(i.radius * 1.75);
            circle.setPosition(i.position);
            circle.setFillColor(i.color);
            window.draw(circle);
        }

        window.display();
    }
 
    void move(bool center, bool explode) {

        for (Particle& i : particles) {

            if (i.fixed) {
                continue;
            }

            sf::Vector2f gravity = gravity_system;
            sf::Vector2f change = i.position - i.previous;
            i.previous = i.position;

            if (center || explode) {
                gravity.x = force * 10 * ((i.position.x / display_x) - 0.5);
                gravity.y = force * 10 * ((i.position.y / display_y) - 0.5);
            }

            if (center) {
                gravity *= float(-1);
            }

            i.position += change + gravity * delta * delta;
        }
    }

    void advance(
        sf::RenderWindow& window,
        bool up,
        bool left,
        bool right,
        bool zero,
        bool center,
        bool explode
    ) {
               
        for (int i = 0; i < substeps; i++) {
            impose_bounds();
            assign_grid();
            collide();
            adjust_gravity(up, left, right, zero);
            move(center, explode);
        }

        render(window);
    }
};
