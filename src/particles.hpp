#pragma once

#include <SFML/Graphics.hpp>
#include "data-reader.hpp"
#include <cmath>
#include <set>

using Key = std::pair<int, int>;
using Softbody = std::map<Key, float>;
using Softbodies = std::map<int, Softbody>;

struct Particle {

    sf::Vector2f position = {1, 1};
    sf::Vector2f previous = {1, 1};

    sf::Color color = {0, 150, 255, 150};
    float radius = 6;

    int linked = -1;
    bool fixed = false;
    bool is_mouse = false;
    
    int current = 0;
    bool fixed_motion = false;
    std::vector<sf::Vector2f> motion;

    int body_id = -1;
    bool softbody = false;
};

struct Particles {

    std::vector<Particle> contents;
    std::vector<int> linked_particles;
    Softbodies softbodies;
    Reader reader;

    void apply_specification() {

        linked_particles.clear();
        FileData data = reader.read_spec();

        std::vector<int> links_to_check;
        int id = 0;

        for (FileRow row : data) {

            Particle particle;

            particle.position.x = row[0];
            particle.previous.x = row[0];
            particle.position.y = row[1];
            particle.previous.y = row[1];
            particle.linked = int(row[2]);
            particle.fixed = 1 == int(row[3]);

            if (particle.linked >= 0) {
                links_to_check.push_back(id);
            }

            contents.push_back(particle);
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

    void apply_motion() {

        FileData data = reader.read_motion();
        
        std::map<int, int> ids;

        for (FileRow row : data) {

            sf::Vector2f location;

            int id = int(row[0]);

            if (ids.count(id) == 0) {
                Particle particle;
                contents.emplace_back(particle);
                ids[id] = contents.size() - 1;
                contents[ids[id]].fixed_motion = true;
            }

            location.x = row[1];
            location.y = row[2];

            contents[ids[id]].motion.emplace_back(location);
        }
    }

    void apply_softbody() {

        softbodies.clear();
        FileData data = reader.read_softbody();

        std::map<int, std::vector<int>> bodies;

        for (FileRow row : data) {

            Particle particle;

            int body_id = int(row[0]);
            int content_id = contents.size();
            
            bodies[body_id].emplace_back(content_id);

            particle.position.x = row[1];
            particle.previous.x = row[1];
            particle.position.y = row[2];
            particle.previous.y = row[2];
            particle.softbody = true;
            particle.body_id = body_id;

            contents.emplace_back(particle);
        }

        for (auto& softbody : bodies) {

            int body_id = softbody.first;
            std::vector<int> indices = softbody.second;

            for (int i = 0; i < indices.size(); i++) {
                for (int j = i + 1; j < indices.size(); j++) {

                    int a = indices[i];
                    int b = indices[j];

                    sf::Vector2f a_pos = contents[a].position;
                    sf::Vector2f b_pos = contents[b].position;
                    sf::Vector2f change = a_pos - b_pos;

                    float dist = sqrt(change.x * change.x + change.y * change.y);

                    softbodies[body_id][{a, b}] = dist;
                }
            }
        }
    }

    void validate(int argc, char* argv[]) {
        reader.validate(argc, argv);
    }

    void reset() {
        contents.clear();
        apply_specification();
        apply_motion();
        apply_softbody();
        reader.first_load = false;
    }
};
