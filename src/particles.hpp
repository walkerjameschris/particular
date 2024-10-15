#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string validate_path(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "You must provide a spec path!\n";
        std::exit(1);
    }

    if (!std::filesystem::exists(argv[1])) {
        std::cout << "File does not exist!\n";
        std::exit(1);
    }

    return argv[1];
}

struct Particle {
    sf::Vector2f position = {1, 1};
    sf::Vector2f previous = {1, 1};
    int linked = -1;
    bool fixed = false;
    float radius = 5;
    float delay = 0;
    sf::Color color = {0, 150, 255, 150};
};

int str_to_int(std::string x, bool& skip) {

    try {
        return std::stoi(x);
    } catch (...) {
        skip = true;
        return 0;
    }
}

float str_to_num(std::string x, bool& skip) {

    try {
        return std::stof(x);
    } catch (...) {
        skip = true;
        return 0;
    }
}

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;

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

            bool skip = false;

            for (int i = 0; i < row.size(); i++) {
                if (i == 0) {
                    particle.position.x = str_to_num(row[i], skip);
                    particle.previous.x = str_to_num(row[i], skip);
                } else if (i == 1) {
                    particle.position.y = str_to_num(row[i], skip);
                    particle.previous.y = str_to_num(row[i], skip);
                } else if (i == 2) {
                    particle.linked = str_to_int(row[i], skip);
                } else if (i == 3) {
                    particle.fixed = 1 == str_to_int(row[i], skip);
                } else if (i == 4) {
                    particle.radius = str_to_num(row[i], skip);
                }
            }

            if (skip) {
                continue;
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
