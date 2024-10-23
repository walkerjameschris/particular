#pragma once
#include <SFML/Graphics.hpp>
#include "data-reader.hpp"
#include <filesystem>
#include <iostream>

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

float str_to_num(std::string x) {

    try {
        return std::stof(x);
    } catch (...) {
        std::cout << "Value cannot be coerced to number!\n";
        std::exit(1);
    }
}

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;

    std::string main_path;
    std::string motion_path;
    bool has_motion = false;

    void validate(int argc, char* argv[]) {

        if (argc < 2) {
            std::cout << "You must provide a main path!\n";
            std::exit(1);
        }

        main_path = argv[1];

        if (!std::filesystem::exists(main_path)) {
            std::cout << "Specification file does not exist!\n";
            std::exit(1);
        }

        if (argc > 2) {
            motion_path = argv[2];
        }

        if (std::filesystem::exists(motion_path)) {
            has_motion = true;
        }
    }

    void reset() {

        contents.clear();
        linked_particles.clear();

        Data data = read_file(main_path);
        std::vector<int> links_to_check;
        int index = 0;

        for (auto row : data) {

            bool skip = false;
            Particle particle;
            std::string nicename = "Specification";

            if (row.size() != 4) {
                std::cout << "Specification file is malformed!\n";
                std::cout << "It must have the following structure:\n";
                std::cout << "x, y, linked, fixed\n";
                std::exit(1);
            }

            particle.position.x = str_to_num(row[0]);
            particle.previous.x = str_to_num(row[0]);
            particle.position.y = str_to_num(row[1]);
            particle.previous.y = str_to_num(row[1]);
            particle.linked = int(str_to_num(row[2]));
            particle.fixed = 1 == int(str_to_num(row[3]));

            if (particle.linked >= 0) {
                links_to_check.push_back(index);
            }

            contents.push_back(particle);
            index += 1;
        }

        Particle mouse;
        mouse.is_mouse = true;
        mouse.radius *= 5;
        mouse.color = {0, 0, 0, 0};
        contents.push_back(mouse);

        for (int i : links_to_check) {
            if (i < contents.size()) {
                linked_particles.push_back(i);
            }
        }

        if (has_motion) {

            data.clear();
            data = read_file(motion_path);

            for (auto row : data) {

                int index = 0;
                sf::Vector2f location;

                 if (row.size() != 3) {
                    std::cout << "Motion file is malformed!\n";
                    std::cout << "It must have the following structure:\n";
                    std::cout << "index, x, y\n";
                    std::exit(1);
                }

                index = int(str_to_num(row[0]));
                location.x = str_to_num(row[1]);
                location.y = str_to_num(row[2]);

                if (index >= contents.size()) {
                    continue;
                }

                Particle& motion_particle = contents[index];
                motion_particle.motion.emplace_back(location);
                motion_particle.fixed_motion = true;
            }
        }
    }
};
