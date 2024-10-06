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
    float delay = 0;
    sf::Color color = {0, 150, 255, 150};
};

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;

    // TODO: Add parser validation and make less clunky
    // This is super sketchy and is asking for a segfault
    void load_spec(std::string path = "") {

        contents.clear();
        linked_particles.clear();
    
        std::vector<std::vector<std::string>> data;
        std::vector<int> link_buffer;
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
        Particle particle;
        int index = 0;

        for (auto row : data) {
            for (int i = 0; i < row.size(); i++) {
                if (i == 0) {
                    particle.position.x = std::stof(row[i]);
                    particle.previous.x = std::stof(row[i]);
                } else if (i == 1) {
                    particle.position.y = std::stof(row[i]);
                    particle.previous.y = std::stof(row[i]);
                } else if (i == 2) {
                    particle.linked = std::stoi(row[i]);
                } else if (i == 3) {
                    particle.fixed = 1 == std::stoi(row[i]);
                } else if (i == 4) {
                    particle.radius = std::stof(row[i]);
                } else if (i == 5) {
                    particle.delay = std::stof(row[i]);
                }
            }

            if (particle.linked >= 0) {
                link_buffer.push_back(index);
            }

            contents.push_back(particle);
            index += 1;
        }

        for (int i : link_buffer) {
            if (i < contents.size()) {
                linked_particles.push_back(i);
            }
        }
    }
};
