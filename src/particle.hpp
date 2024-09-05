#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <thread>

struct Color {
    int r = 000;
    int b = 255;
    int g = 050;
    int a = 150;
    bool ascending = true;
};

struct Particles {

    int n_particle;
    int n_grid_x;
    int n_grid_y;
    int display_x;
    int display_y;
    float width;
    float radius;
    float max_shift;

    Color color;

    std::vector<float> x_pos;
    std::vector<float> y_pos;
    std::vector<float> x_prv;
    std::vector<float> y_prv;
    std::vector<Color> colors;

    std::unordered_map<int, std::vector<int>> grid;

    //// Particle Utilities ////

    float clamp(float x, float min, float max) {
        // Bounds a float between limits
    
        if (x > max) {
            return max;
        }

        if (x < min) {
            return min;
        }

        return x;
    }

    int id_raw(int x, int y) {
        // Hashing function to create grid keys

        return x * 73856093 ^ y * 19349663;
    }

    int id(float x, float y) {
        // Assigns particle to a grid key

        int x_id = floor(x / width);
        int y_id = floor(y / width);
        return id_raw(x_id, y_id);
    }

    //// Stateful Functions ////

    void color_gen() {
        // Moves the current color through RGB space

        if (color.ascending && color.g < 150) {
            color.g += 1;
        }

        if (color.ascending && color.g == 150) {
            color.ascending = false;
            color.g -= 1;
        }

        if (!color.ascending && color.g > 50) {
            color.g -= 1;
        }

        if (!color.ascending && color.g == 50) {
            color.ascending = true;
            color.g += 1;
        }
    }

    void generate() {
        // Generates a new particle and adds to collection state

        if (x_pos.size() < n_particle) {

            x_pos.emplace_back(1.5);
            y_pos.emplace_back(100.5);
            x_prv.emplace_back(0);
            y_prv.emplace_back(100);
            colors.emplace_back(color);

            color_gen();
        }
    }

    void impose_bounds() {
        // Clamps all particles within display region

        for (int i = 0; i < x_pos.size(); i++) {
            x_pos[i] = clamp(x_pos[i], 0, display_x - radius * 2);
            y_pos[i] = clamp(y_pos[i], 0, display_y - radius * 2);
        }
    }

    void assign_grid() {
        // Assigns all points to the simulation subspace grid

        grid.clear();

        for (int i = 0; i < x_pos.size(); i++) {
            grid[id(x_pos[i], y_pos[i])].emplace_back(i);
        }
    }

    static void collide_inner(
        int inner_id,
        int outer_id,
        std::unordered_map<int, std::vector<int>> grid,
        std::vector<float> x_pos,
        std::vector<float> y_pos
    ) {
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

                int i = inner[a];
                int j = outer[b];

                float x_chg = x_pos[i] - x_pos[j];
                float y_chg = y_pos[i] - y_pos[j];
                float dist_sqrd = x_chg * x_chg + y_chg * y_chg;

                if (dist_sqrd < (radius * 2) * (radius * 2)) {

                    float dist = sqrt(dist_sqrd);
                    float delta = 0.5 * (dist - (radius * 2));
                    float x_div = x_chg / dist;
                    float y_div = y_chg / dist;

                    x_pos[i] -= x_div * delta; // , -max_shift, max_shift);
                    y_pos[i] -= y_div * delta; // , -max_shift, max_shift);
                    x_pos[j] += x_div * delta; // , -max_shift, max_shift);
                    y_pos[j] += y_div * delta; // , -max_shift, max_shift);
                }
            }
        }
    }

    static void collide_thread(
        int start_x,
        int end_x,
        int n_grid_y,
        std::unordered_map<int, std::vector<int>> grid,
        std::vector<float> x_pos,
        std::vector<float> y_pos
    ) {
        // Iterates through the simulation space grid. This
        // cycles through all grid cells and detects collision
        // between the current cell and all neighboring cells.

        for (int i = start_x; i < end_x; i++) {
            for (int j = 0; j < n_grid_y; j++) {
                for (int a = i - 1; a < (i + 1); a++) {
                    for (int b = j - 1; b < (j + 1); b++) {
                        collide_inner(
                            id_raw(i, j),
                            id_raw(a, b),
                            grid,
                            x_pos,
                            y_pos
                        );
                    }
                }
            }
        }
    }

    void collide_grid() {

        int half_point = floor(n_grid_x / 2);

        std::thread thread_1(
            collide_thread,
            0, half_point,
            grid,
            x_pos,
            y_pos
        );

        std::thread thread_2(
            collide_thread,
            half_point,
            n_grid_x,
            grid,
            x_pos,
            y_pos
        );

        thread_1.join();
        thread_2.join();
    }

    void move(float gravity_x, float gravity_y, float dt) {
        // Moves particles with dynamic gravitational force

        for (int i = 0; i < x_pos.size(); i++) {

            float x_chg = x_pos[i] - x_prv[i];
            float y_chg = y_pos[i] - y_prv[i];

            x_prv[i] = x_pos[i];
            y_prv[i] = y_pos[i];

            x_pos[i] += x_chg + gravity_x * dt * dt;
            y_pos[i] += y_chg + gravity_y * dt * dt;
        }
    }

    void render(sf::RenderWindow& window) {
        // Rendering utility to display particles in color

        sf::CircleShape circle(radius * 2);
        circle.setPointCount(32);

        for (int i = 0; i < x_pos.size(); i++) {

            float x = x_pos[i];
            float y = y_pos[i];
            circle.setPosition(x, y);

            sf::Color particle_color(
                colors[i].r,
                colors[i].g,
                colors[i].b,
                colors[i].a
            );

            circle.setFillColor(particle_color);
            window.draw(circle);
        }
    }
};
