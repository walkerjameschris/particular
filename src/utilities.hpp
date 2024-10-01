#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

float unif() {
    return rand() / float(RAND_MAX);
}

float clamp(float x, float min, float max) {
    return std::min(max, std::max(min, x));
}

int hash(int x, int y) {
    return x * 73856093 ^ y * 19349663;
}

int hash_id(int x, int y, float width) {
    int x_id = floor(x / width);
    int y_id = floor(y / width);
    return hash(x_id, y_id);
}

struct Particle {
    sf::Vector2f position = {1, 1};
    sf::Vector2f previous = {1, 1};
    int linked = -1;
    bool fixed = false;
    float radius = 5;
    sf::Color color = {0, 150, 255, 100};
};

using Particles = std::vector<Particle>;

Particles load_spec(std::string path = "") {

    Particles result;
    std::ifstream file_buffer(path);

    std::string line;
    std::vector<std::vector<std::string>> data;

    while (std::getline(file_buffer, line)) {

        std::stringstream stream(line);
        std::string value;
        std::vector<std::string> row;

        while (std::getline(stream, value, ',')) {
            row.push_back(value);
        }

        data.push_back(row);
    }

    file_buffer.close();

    for (auto row : data) {

        Particle new_particle;

        for (int i = 0; i < row.size(); i++) {

            std::string x = row[i];

            if (i == 0) {
                new_particle.position.x = std::stof(x);
                new_particle.previous.x = std::stof(x);
            } else if (i == 1) {
                new_particle.position.y = std::stof(x);
                new_particle.previous.y = std::stof(x);
            } else if (i == 2) {
                new_particle.linked = std::stoi(x);
            } else if (i == 3) {
                new_particle.fixed = 1 == std::stoi(x);
            }
        }

        result.push_back(new_particle);
    }

    return result;
}
