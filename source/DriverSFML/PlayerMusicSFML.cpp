#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

#include "DriverSFML/PlayerMusicSFML.hpp"

namespace SuperHaxagon {
	PlayerMusicSFML::PlayerMusicSFML(std::unique_ptr<sf::Music> music) : _music(std::move(music)) {}

	PlayerMusicSFML::~PlayerMusicSFML() {
		_music->stop();
	}

	void PlayerMusicSFML::setLoop(const bool loop) {
		_music->setLoop(loop);
	}

	void PlayerMusicSFML::play() {
		_music->play();
	}

	bool PlayerMusicSFML::isDone() {
		return _music->getStatus() == sf::SoundSource::Stopped;
	}

	double PlayerMusicSFML::getVelocity() {
		return 0;
	}
}