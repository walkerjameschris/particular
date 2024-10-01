#include <SFML/Graphics.hpp>
#include "src/fps_counter.hpp"
#include "src/simulation.hpp"
#include "src/particles.hpp"
#include "src/utilities.hpp"
#include <string>

int main(int argc, char* argv[]) {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    int outcome = 0;

    std::string path = validate_path(outcome, argc, argv);

    if (outcome == 1) {
        return 1;  
    }

    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Particular");
    sf::Clock clock;
    sf::Event event;

    Simulation simulation(display_x, display_y, frame_rate, path);

    window.setFramerateLimit(frame_rate);

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.advance(
            window,
            clock,
            sf::Keyboard::isKeyPressed(sf::Keyboard::W),
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Z),
            sf::Keyboard::isKeyPressed(sf::Keyboard::C),
            sf::Keyboard::isKeyPressed(sf::Keyboard::X)
        );
    }

    return 0;
}
