#ifndef SUPER_HAXAGON_GAME_HPP
#define SUPER_HAXAGON_GAME_HPP

#include <memory>
#include <vector>
#include <string>

namespace SuperHaxagon {
	// Maybe I went a bit overboard with PImpl...
	struct Color;
	class LevelFactory;
	class AudioLoader;
	class State;
	class Platform;
	class Twist;
	class Font;
	class Metadata;
	enum class Location;

	class Game {
	public:
		explicit Game(Platform& platform);
		Game(const Game&) = delete;
		~Game();

		void run();

		void loadBGMAudio(const std::string& music, Location location, bool loadMetadata);

		void addLevel(std::unique_ptr<LevelFactory> level);
		const std::vector<std::unique_ptr<LevelFactory>>& getLevels() const {return _levels;}

		Platform& getPlatform() const {return _platform;}
		Twist& getTwister() const {return *_twister;}
		AudioLoader& getSFXBegin() const {return *_sfxBegin;}
		AudioLoader& getSFXHexagon() const {return *_sfxHexagon;}
		AudioLoader& getSFXOver() const {return *_sfxOver;}
		AudioLoader& getSFXSelect() const {return *_sfxSelect;}
		AudioLoader& getSFXLevelUp() const {return *_sfxLevelUp;}
		AudioLoader& getSFXWonderful() const {return *_sfxWonderful;}
		AudioLoader* getBGMAudio() const {return _bgmAudio.get();}
		Metadata* getBGMMetadata() const {return _bgmMetadata.get();}
		void setRunning(const bool running) {_running = running;}
		Font& getFontSmall() const {return *_small;}
		Font& getFontLarge() const {return *_large;}

	private:
		Platform& _platform;

		std::vector<std::unique_ptr<LevelFactory>> _levels;

		std::unique_ptr<Twist> _twister;
		std::unique_ptr<State> _state;

		// Should really be an array of sfx
		std::unique_ptr<AudioLoader> _sfxBegin;
		std::unique_ptr<AudioLoader> _sfxHexagon;
		std::unique_ptr<AudioLoader> _sfxOver;
		std::unique_ptr<AudioLoader> _sfxSelect;
		std::unique_ptr<AudioLoader> _sfxLevelUp;
		std::unique_ptr<AudioLoader> _sfxWonderful;
		
		std::unique_ptr<AudioLoader> _bgmAudio;
		std::unique_ptr<Metadata> _bgmMetadata;
		
		std::unique_ptr<Font> _small;
		std::unique_ptr<Font> _large;

		bool _running = true;
	};
}

#endif //SUPER_HAXAGON_GAME_HPP
