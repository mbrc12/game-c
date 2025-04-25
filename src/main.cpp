#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cpp-dump/cpp-dump.hpp>

constexpr int fps = 60;
constexpr int viewport_width = 640;
constexpr int viewport_height = 360;
constexpr int window_width = 800;
constexpr int window_height = 600;

constexpr sf::Time per_frame = sf::seconds(1.0f / fps);

// allow cpp-dump to print sf::Vector2f
CPP_DUMP_DEFINE_EXPORT_OBJECT(sf::Vector2f, x, y);

int main() {
	// Enable for fullscreen mode
	//auto mode = sf::VideoMode::getFullscreenModes()[0];
	//sf::RenderWindow window(mode, "Game", sf::Style::None, sf::State::Fullscreen);

	// Enable for windowed mode
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Game", sf::Style::Default, sf::State::Windowed);

	window.setVerticalSyncEnabled(true);

	sf::RenderTexture viewport({viewport_width, viewport_height});

	sf::Clock clock;

	float scale; sf::Vector2f position = {0, 0}; // variables for viewport position and scale, modified by resizer

	auto resizer = [&]() {
		auto [width, height] = window.getSize();
		scale = std::min(width * 1.0f / viewport_width, height * 1.0f / viewport_height);
		position = sf::Vector2f((width - viewport_width * scale) / 2, (height - viewport_height * scale) / 2);
		sf::View view(sf::FloatRect({0, 0}, {width * 1.0f, height * 1.0f}));
		window.setView(view);

		cpp_dump(position);
	};

	resizer();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
			}
			if (event->is<sf::Event::Resized>()) {
				resizer();
			}
		}

		viewport.clear(sf::Color::Yellow);
		sf::CircleShape circle(100);
		circle.setFillColor(sf::Color::Green);
		circle.setPosition({-30, -30});
		viewport.draw(circle);
		viewport.display();

		sf::Sprite sprite(viewport.getTexture());
		sprite.setScale({scale, scale});
		sprite.setPosition(position);

		window.clear();
		window.draw(sprite);
		window.display();

		auto elapsed = clock.restart();
		if (elapsed < per_frame) {
			sf::sleep(per_frame - elapsed);
		}
    }
}
