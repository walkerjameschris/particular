#pragma once
#include <SFML/Graphics.hpp>
#include "fps_counter.hpp"
#include "particles.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <map>

using Key = std::pair<int, int>;

struct Simulation {

    Particles particles;
    sf::CircleShape circle;
    std::map<Key, std::vector<int>> grid;
    sf::Vector2f system_gravity;
    std::string path;
    FPS fps_counter;
    
    float display_x;
    float display_y;
    float delta;

    int substeps = 3;
    float force = 1000;
    float width = 18;

    Simulation(int x, int y, int fps, std::string file) {
        particles.load_spec(file);
        display_x = x;
        display_y = y;
        delta = 1 / float(fps * substeps);
        path = file;
    }

    void impose_bounds() {
        for (Particle& i : particles.contents) {
            i.position.x = std::clamp(i.position.x, 0.f, display_x - i.radius);
            i.position.y = std::clamp(i.position.y, 0.f, display_y - i.radius);
        }
    }

    void assign_grid() {

        grid.clear();
        int index = 0;

        for (Particle& i : particles.contents) {
            int x_id = floor(i.position.x / width);
            int y_id = floor(i.position.y / width);
            grid[{x_id, y_id}].emplace_back(index);
            index += 1;
        }
    }

    void verlet(Particle& i, Particle& j, bool linked) {

        sf::Vector2f change = i.position - j.position;

        if (change.x == 0 && change.y == 0) {
            return;
        }

        float d_sqrd = change.x * change.x + change.y * change.y;
        float tolerance = i.radius + j.radius;

        if ((d_sqrd < tolerance * tolerance) || linked) {

            float distance = sqrt(d_sqrd);
            float i_ratio = i.radius / tolerance;
            float j_ratio = j.radius / tolerance;

            float scalar = 0.5 * (distance - tolerance);
            sf::Vector2f divisor = scalar * change / distance;

            if (!i.fixed) {
                i.position -= divisor * i_ratio;
            }

            if (!j.fixed) {
                j.position += divisor * j_ratio;
            }
        }
    }

    void collide_inner_grid(Key inner_id, Key outer_id) {

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

                Particle& i = particles.contents[i_id];
                Particle& j = particles.contents[j_id];

                if (i.linked == j_id || j.linked == i_id) {
                    continue;
                }
                
                verlet(i, j, false);
            }
        }
    }

    void collide_grid() {

        for (auto& element : grid) {
            Key x = element.first;
            for (int a = x.first - 1; a < (x.first + 1); a++) {
                for (int b = x.second - 1; b < (x.second + 1); b++) {
                    collide_inner_grid({x.first, x.second}, {a, b});
                }
            }
        }
    }

    void collide_linked(bool unlink) {

        if (unlink) {
            return;
        }

        for (int index : particles.linked_particles) {
            Particle& i = particles.contents[index];
            Particle& j = particles.contents[i.linked];
            verlet(i, j, true);
        }
    }

    void adjust_gravity(bool up, bool left, bool right, bool zero) {

        system_gravity = {0, force};
        
        if (zero) {
            system_gravity = {0, 0};
        } else if (up) {
            system_gravity = {0, -force};
        } else if (left) {
            system_gravity = {-force, 0};
        } else if (right) {
            system_gravity = {force, 0};
        }
    }
    
    void render(sf::RenderWindow& window, sf::Clock& clock) {

        window.clear();

        for (Particle& i : particles.contents) {
            circle.setPointCount(32);
            circle.setRadius(i.radius * 1.5);
            circle.setPosition(i.position);
            circle.setFillColor(i.color);
            window.draw(circle);
        }

        fps_counter.render(window, clock);
        window.display();
    }
 
    void move(bool center, bool explode) {

        for (Particle& i : particles.contents) {

            if (i.fixed) {
                continue;
            }

            sf::Vector2f gravity = system_gravity;
            sf::Vector2f change = i.position - i.previous;
            i.previous = i.position;

            if (center || explode) {
                gravity.x = force * 10 * ((i.position.x / display_x) - 0.5);
                gravity.y = force * 10 * ((i.position.y / display_y) - 0.5);
            }

            if (center) {
                gravity *= -1.0f;
            }

            i.position += change + gravity * delta * delta;
        }
    }

    void advance(
        sf::RenderWindow& window,
        sf::Clock& clock,
        bool up,
        bool left,
        bool right,
        bool zero,
        bool center,
        bool explode,
        bool reset,
        bool unlink
    ) {

        if (reset) {
            particles.load_spec(path);
        }
               
        for (int i = 0; i < substeps; i++) {
            impose_bounds();
            assign_grid();
            collide_grid();
            collide_linked(unlink);
            adjust_gravity(up, left, right, zero);
            move(center, explode);
        }

        render(window, clock);
    }
};
