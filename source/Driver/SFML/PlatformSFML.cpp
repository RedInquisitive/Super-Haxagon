#include "Driver/SFML/PlatformSFML.hpp"

#include "Core/SurfaceUI.hpp"
#include "Core/SurfaceGame.hpp"
#include "Driver/SFML/AudioLoaderSFML.hpp"
#include "Driver/SFML/FontSFML.hpp"
#include "Driver/SFML/AudioPlayerSoundSFML.hpp"
#include "Driver/SFML/SurfaceSFML.hpp"

#include <string>

namespace SuperHaxagon {
	PlatformSFML::PlatformSFML(const Dbg dbg, sf::VideoMode video) : Platform(dbg) {
		_clock.restart();
		auto surface = std::make_unique<SurfaceSFML>(video);
		_window = surface->getWindow();
		_surface = std::move(surface);
		_surfaceUI = std::make_unique<SurfaceUI>(_surface.get());
		_surfaceGame = std::make_unique<SurfaceGame>(_surface.get());
		_surfaceGameShadows = std::make_unique<SurfaceGame>(_surface.get());
		_loaded = true;
	}

	PlatformSFML::~PlatformSFML() = default;

	bool PlatformSFML::loop() {
		const auto throttle = sf::milliseconds(1);
		sf::sleep(throttle);
		_delta = _clock.getElapsedTime().asSeconds();
		_clock.restart();
		sf::Event event{};
		while (_window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) _window->close();
			if (event.type == sf::Event::GainedFocus) _focus = true;
			if (event.type == sf::Event::LostFocus) _focus = false;
			if (event.type == sf::Event::Resized) {
				const auto width = event.size.width > 400 ? event.size.width : 400;
				const auto height = event.size.height > 240 ? event.size.height : 240;

				if (width != event.size.width || height != event.size.height) {
					_window->setSize({ width, height });
				}
				
				sf::FloatRect visibleArea(
					0.0f, 
					0.0f, 
					static_cast<float>(width), 
					static_cast<float>(height)
				);

				_window->setView(sf::View(visibleArea));
			}
		}
		return _loaded && _window->isOpen();
	}

	float PlatformSFML::getDilation() {
		// The game was originally designed with 60FPS in mind
		const auto dilation = _delta / (1.0f / 60.0f);
		return dilation > 4.0f ? 4.0f : (dilation < 0.05f ? 0.05f : dilation);
	}

	std::unique_ptr<AudioLoader> PlatformSFML::loadAudio(const std::string& path, Stream stream, Location location) {
		return std::make_unique<AudioLoaderSFML>(getPath(path, location), stream);
	}

	std::unique_ptr<Font> PlatformSFML::loadFont(const std::string& path, const int size) {
		auto* surface = static_cast<SurfaceSFML*>(_surface.get());
		return std::make_unique<FontSFML>(surface, path, static_cast<float>(size));
	}

	void PlatformSFML::playSFX(AudioLoader& audio) {
		for (auto it = _sfx.begin(); it != _sfx.end();) {
			auto& playing = *it;
			if (playing->isDone()) {
				it = _sfx.erase(it);
			} else {
				++it;
			}
		}

		auto player = audio.instantiate();
		if (!player) return;
		player->setLoop(false);
		player->play();
		_sfx.emplace_back(std::move(player));
	}

	void PlatformSFML::playBGM(AudioLoader& audio) {
		_bgm = audio.instantiate();
		if (!_bgm) return;
		_bgm->setLoop(true);
		_bgm->play();
	}

	std::string PlatformSFML::getButtonName(const Buttons& button) {
		if (button.back) return "ESC";
		if (button.select) return "ENTER";
		if (button.left) return "LEFT";
		if (button.right) return "RIGHT";
		if (button.quit) return "DELETE";
		return "?";
	}

	Buttons PlatformSFML::getPressed() {
		Buttons buttons{};
		if (!_focus) return buttons;
		buttons.select = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);
		buttons.back = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
		buttons.quit = sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);
		buttons.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		buttons.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		return buttons;
	}
}
