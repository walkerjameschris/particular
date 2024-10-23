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

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;

    std::string spec_path;
    std::string motion_path;
    bool has_motion = false;

    std::string current_file;
    int index = 1;

    void validate(int argc, char* argv[]) {

        if (argc < 2) {
            std::cout << "You must provide a!\n";
            std::exit(1);
        }

        spec_path = argv[1];

        if (!std::filesystem::exists(spec_path)) {
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

    float str_to_num(std::string x) {

        try {
            return std::stof(x);
        } catch (...) {
            std::cout << "Problem found on line " + std::to_string(index);
            std::cout << " of the " + current_file + " file: " + x + "\n";
            std::exit(1);
        }
    }

    void reset() {

        contents.clear();
        linked_particles.clear();

        Data data = read_file(spec_path);
        std::vector<int> links_to_check;
        int id = 0;

        for (Row row : data) {

            bool skip = false;
            Particle particle;
            current_file = "specifcation";

            if (row.size() != 4) {
                std::cout << "Specification file is malformed on line ";
                std::cout << std::to_string(index) + "!\n";
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
                links_to_check.push_back(id);
            }

            contents.push_back(particle);
            index += 1;
            id += 1;
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
            current_file = "motion";
            index = 1;

            for (Row row : data) {

                sf::Vector2f location;

                 if (row.size() != 3) {
                    std::cout << "Motion file is malformed on line ";
                    std::cout << std::to_string(index) + "!\n";
                    std::cout << "It must have the following structure:\n";
                    std::cout << "id, x, y\n";
                    std::exit(1);
                }

                id = int(str_to_num(row[0]));
                location.x = str_to_num(row[1]);
                location.y = str_to_num(row[2]);

                if (id >= contents.size()) {
                    continue;
                }

                contents[id].motion.emplace_back(location);
                contents[id].fixed_motion = true;
            }
        }
    }
};
