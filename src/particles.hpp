#pragma once
#include <SFML/Graphics.hpp>
#include "data-reader.hpp"
#include <filesystem>
#include <iostream>
#include <cmath>
#include <set>

using Key = std::pair<int, int>;

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
    bool softbody = false;
    int body_id = -1;
    std::vector<sf::Vector2f> motion;
};

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;
    std::map<int, std::vector<int>> softbodies;
    std::map<Key, float> softbody_distances;

    std::string spec_path;
    std::string motion_path;
    std::string softbody_path;

    bool has_spec = true;
    bool has_motion = true;
    bool has_softbody = true;

    std::string current_file;
    int index = 1;

    void check_path(
        std::string path,
        std::string file,
        bool& indicator
    ) {

        if (path != "--pass") {
            if (std::filesystem::exists(path)) {
                indicator = true;
            } else {
                std::cout << file + " path does not exist!\n";
                std::cout << "Tip: you can skip with '--pass'\n";
                std::exit(1);
            }
        }
    }

    void validate(int argc, char* argv[]) {

        if (argc < 4) {
            std::cout << "You must provide '--pass' or a path!\n";
            std::cout << "The commands follow this order:\n";
            std::cout << "Specification, Motion, Softbody\n\n";
            std::cout << "Tip: ./particular <a path> --pass -pass\n";
            std::exit(1);
        }

        spec_path = argv[1];
        motion_path = argv[2];
        softbody_path = argv[3];
        
        check_path(spec_path, "Specification", has_spec);
        check_path(motion_path, "Motion", has_motion);
        check_path(softbody_path, "Softbody", has_softbody);
    }

    void check_row(Row row, int size, std::string structure) {

        if (row.size() != size) {
            std::cout << current_file + " file is malformed on line ";
            std::cout << std::to_string(index) + "!\n";
            std::cout << "It must have the following structure:\n";
            std::cout << structure + "\n";
            std::exit(1);
        }
    }

    float str_to_num(std::string x) {

        try {
            return std::stof(x);
        } catch (...) {
            std::cout << current_file + " file is malformed on ";
            std::cout << "line " + std::to_string(index) + "\n";
            std::cout << "Cannot convert value to number!\n";
            std::exit(1);
        }
    }

    float get_distance(Particle& a, Particle& b) {
        sf::Vector2f change = a.position - b.position;
        return sqrt(change.x * change.x + change.y * change.y);
    }

    void reset() {

        contents.clear();
        linked_particles.clear();
        softbodies.clear();
        softbody_distances.clear();

        Data data = read_file(spec_path);

        if (has_spec) {

            std::vector<int> links_to_check;
            int id = 0;

            for (Row row : data) {

                bool skip = false;
                Particle particle;
                current_file = "Specifcation";

                check_row(row, 4, "x, y, linked-id, fixed-bool");

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

        }

        if (has_motion) {

            data.clear();
            data = read_file(motion_path);
            current_file = "Motion";
            index = 1;

            std::map<int, int> ids;

            for (Row row : data) {

                sf::Vector2f location;

                check_row(row, 3, "path-id, x, y");

                int id = int(str_to_num(row[0]));

                if (ids.count(id) == 0) {
                    Particle particle;
                    contents.emplace_back(particle);
                    ids[id] = contents.size() - 1;
                    contents[ids[id]].fixed_motion = true;
                }

                location.x = str_to_num(row[1]);
                location.y = str_to_num(row[2]);

                contents[ids[id]].motion.emplace_back(location);
            }
        }

        if (has_softbody) {

            data.clear();
            data = read_file(softbody_path);
            current_file = "Softbody";
            index = 1;

            for (Row row : data) {

                check_row(row, 3, "shape-id, x, y");

                Particle particle;

                int body_id = int(str_to_num(row[0]));
                int content_id = contents.size();
                
                softbodies[body_id].emplace_back(content_id);

                particle.position.x = str_to_num(row[1]);
                particle.previous.x = str_to_num(row[1]);
                particle.position.y = str_to_num(row[2]);
                particle.previous.y = str_to_num(row[2]);
                particle.softbody = true;
                particle.body_id = body_id;

                index += 1;

                contents.emplace_back(particle);
            }

            for (auto& softbody : softbodies) {

                std::vector<int> indices = softbody.second;

                for (int i = 0; i < indices.size(); i++) {
                    for (int j = i + 1; j < indices.size(); j++) {

                        int a = indices[i];
                        int b = indices[j];

                        float dist = get_distance(contents[a], contents[b]);

                        softbody_distances[{a, b}] = dist;
                    }
                }
            }
        }
    }
};
