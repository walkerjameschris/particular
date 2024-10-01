#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Particle {
    sf::Vector2f position = {1, 1};
    sf::Vector2f previous = {1, 1};
    int linked = -1;
    bool fixed = false;
    float radius = 5;
    sf::Color color = {0, 150, 255, 100};
};

struct Particles {

    std::vector<Particle> contents;
    bool use_grid = true;

    void load_spec(std::string path = "") {
    
        std::vector<std::vector<std::string>> data;
        std::ifstream file_buffer(path);
        std::string line;

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

            if (new_particle.linked >= 0) {
                use_grid = false;
            }

            contents.push_back(new_particle);
        }
    }
};
