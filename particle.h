#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <unordered_map>

struct Particles {

    float width;
    std::vector<float> x_pos;
    std::vector<float> y_pos;
    std::vector<float> x_prv;
    std::vector<float> y_prv;
    std::unordered_map<int, std::vector<int>> grid;

    int id_raw(int x, int y) {
        return x * 73856093 ^ y * 19349663;
    }

    int id(float x, float y) {
        int x_id = floor(x / width);
        int y_id = floor(y / width);
        return id_raw(x_id, y_id);
    }

    void generate(sf::Clock& clock, int n, float delay) {

        float elapsed = clock.getElapsedTime().asSeconds();

        if (x_pos.size() < n && elapsed >= delay) {

            x_pos.emplace_back(251);
            y_pos.emplace_back(100);
            x_prv.emplace_back(250);
            y_prv.emplace_back(100);

            clock.restart();
        }
    }

    void impose_bounds(float display, float radius) {

        for (int i = 0; i < x_pos.size(); i++) {

            // float x_dist = center - x_pos[i];
            // float y_dist = center - y_pos[i];
            // float dist = sqrt(x_dist * x_dist + y_dist * y_dist);

            // if (dist > bound) {
            //     x_pos[i] = center - (x_dist / dist) * bound;
            //     y_pos[i] = center - (y_dist / dist) * bound;
            // }

            if (x_pos[i] < 0) {
                x_pos[i] = 0;
            }

            if (y_pos[i] < 0) {
                y_pos[i] = 0;
            }

            if (x_pos[i] > (display - radius)) {
                x_pos[i] = (display - radius);
            }

            if (y_pos[i] > (display - radius)) {
                y_pos[i] = (display - radius);
            }
        }
    }

    void assign_grid() {

        grid.clear();

        for (int i = 0; i < x_pos.size(); i++) {
            grid[id(x_pos[i], y_pos[i])].emplace_back(i);
        }
    }

    void collide_inner(
        int inner_id,
        int outer_id,
        float throttle,
        float radius,
        float display
    ) {

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
                    float delta = throttle * (dist - (radius * 2));
                    float x_div = x_chg / dist;
                    float y_div = y_chg / dist;

                    x_pos[i] -= x_div * delta;
                    y_pos[i] -= y_div * delta;
                    x_pos[j] += x_div * delta;
                    y_pos[j] += y_div * delta;
                }
            }
        }
    }

    void collide_grid(float throttle, float radius, float display) {
        for (int i = 0; i < ceil(display / width); i++) {
            for (int j = 0; j < ceil(display / width); j++) {
                for (int a = i - 1; a < (i + 1); a++) {
                    for (int b = j - 1; b < (j + 1); b++) {
                        collide_inner(
                            id_raw(i, j),
                            id_raw(a, b),
                            throttle,
                            radius,
                            display
                        );
                    }
                }
            }
        }
    }

    void move(float gravity, float dt) {

        for (int i = 0; i < x_pos.size(); i++) {

            float x_chg = x_pos[i] - x_prv[i];
            float y_chg = y_pos[i] - y_prv[i];

            x_prv[i] = x_pos[i];
            y_prv[i] = y_pos[i];

            x_pos[i] += x_chg;
            y_pos[i] += y_chg + gravity * dt * dt;
        }
    }

    void render(sf::RenderWindow& window, float radius) {

        window.clear();
        sf::CircleShape circle(radius);
        circle.setPointCount(32);
        circle.setFillColor(sf::Color::White);

        for (int i = 0; i < x_pos.size(); i++) {

            float x = x_pos[i];
            float y = y_pos[i];
            circle.setPosition(x, y);
            window.draw(circle);
        }

		window.display();
    }
};