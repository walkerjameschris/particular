#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void validate_path(
    int argc,
    char* argv[],
    std::string& path,
    std::string& motion,
    bool& has_motion
) {

    if (argc < 2) {
        std::cout << "You must provide a spec path!\n";
        std::exit(1);
    }

    path = argv[1];

    if (!std::filesystem::exists(path)) {
        std::cout << "Specification file does not exist!\n";
        std::exit(1);
    }

    if (argc > 2) {
        motion = argv[2];
    }

    if (std::filesystem::exists(motion)) {
        has_motion = true;
    }
}

struct Particle {

    sf::Vector2f position = {1, 1};
    sf::Vector2f previous = {1, 1};

    sf::Color color = {0, 150, 255, 150};
    float radius = 5;

    int linked = -1;
    bool fixed = false;
    bool is_mouse = false;

    int current = 0;
    bool fixed_motion = false;
    std::vector<sf::Vector2f> motion;
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

    void load_spec(
        std::string path,
        std::string motion,
        bool has_motion
    ) {

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
        int index = 0;

        for (auto row : data) {

            bool skip = false;
            Particle particle;

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

        Particle mouse;
        mouse.is_mouse = true;
        mouse.radius *= 5;
        mouse.color = {0, 0, 0, 0};
        contents.push_back(mouse);

        for (int i : link_buffer) {
            if (i < contents.size()) {
                linked_particles.push_back(i);
            }
        }

        if (has_motion) {

            data.clear();
            std::ifstream file_buffer(motion);
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

                int index = 0;
                bool skip = false;
                sf::Vector2f location;

                for (int i = 0; i < row.size(); i++) {
                    if (i == 0) {
                        index = str_to_int(row[i], skip);
                    } else if (i == 1) {
                        location.x = str_to_num(row[i], skip);
                    } else if (i == 2) {
                        location.y = str_to_num(row[i], skip);
                    }
                }

                if (index >= contents.size()) {
                    continue;
                }

                contents[index].motion.emplace_back(location);
                contents[index].fixed_motion = true;
            }
        }
    }
};
