#include "Core/Game.hpp"

#include "Core/Metadata.hpp"
#include "Core/Twist.hpp"
#include "Core/Platform.hpp"
#include "Core/Surface.hpp"
#include "States/Load.hpp"
#include "Factories/LevelFactory.hpp"

namespace SuperHaxagon {
	Game::Game(Platform& platform) : _platform(platform) {
		// Audio loading
		_sfxBegin = platform.loadAudio("/sound/begin", Stream::DIRECT, Location::ROM);
		_sfxHexagon = platform.loadAudio("/sound/hexagon", Stream::DIRECT, Location::ROM);
		_sfxOver = platform.loadAudio("/sound/over", Stream::DIRECT, Location::ROM);
		_sfxSelect = platform.loadAudio("/sound/select", Stream::DIRECT, Location::ROM);
		_sfxLevelUp = platform.loadAudio("/sound/level", Stream::DIRECT, Location::ROM);
		_sfxWonderful = platform.loadAudio("/sound/wonderful", Stream::DIRECT, Location::ROM);

		_small = platform.loadFont(platform.getPath("/bump-it-up", Location::ROM), 16);
		_large = platform.loadFont(platform.getPath("/bump-it-up", Location::ROM), 32);

		_twister = platform.getTwister();
	}

	Game::~Game() {
		_platform.stopBGM();
		_platform.message(Dbg::INFO, "game", "shutdown ok");
	}

	void Game::run() {
		_state = std::make_unique<Load>(*this);
		_state->enter();
		while(_running && _platform.loop()) {
			const auto dilation = _platform.getDilation();
			auto& surface = _platform.getSurface();

			auto next = _state->update(dilation);
			if (!_running) break;
			while (next) {
				_state->exit();
				_state = std::move(next);
				_state->enter();
				next = _state->update(dilation);
			}

			_state->drawTop(_platform.getSurfaceGame(), _platform.getSurfaceGameShadows());
			_state->drawTopUI(_platform.getSurfaceUI());
			surface.screenSwitch();
			_state->drawBotUI(_platform.getSurfaceUI());
			surface.screenFinalize();
		}
	}

	void Game::loadBGMAudio(const std::string& music, const Location location, const bool loadMetadata) {
		const auto base = "/bgm" + music;

		if (loadMetadata) {
			_bgmMetadata = std::make_unique<Metadata>(_platform.openFile(base + ".txt", location));
		}
		
		_bgmAudio = _platform.loadAudio(base, Stream::INDIRECT, location);
		_platform.playBGM(*getBGMAudio());
	}

	void Game::addLevel(std::unique_ptr<LevelFactory> level) {
		_levels.emplace_back(std::move(level));
	}
}
