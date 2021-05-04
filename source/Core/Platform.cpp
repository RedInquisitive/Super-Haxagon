#include "Core/Platform.hpp"

#include "Core/AudioPlayer.hpp"
#include "Core/Surface.hpp"
#include "Core/SurfaceUI.hpp"
#include "Core/SurfaceGame.hpp"

#include <fstream>

namespace SuperHaxagon {
	Platform::Platform(const Dbg dbg) : _dbg(dbg) {}
	Platform::~Platform() = default;

	std::unique_ptr<std::istream> Platform::openFile(const std::string& partial, const Location location) {
		return std::make_unique<std::ifstream>(getPath(partial, location), std::ios::in | std::ios::binary);
	}

	void Platform::stopBGM() {
		_bgm = nullptr;
	}
	
	AudioPlayer* Platform::getBGM() {
		return _bgm.get();
	}

	Supports Platform::supports() {
		// By default support everything. Individual platforms can turn features off.
		return Supports::FILESYSTEM | Supports::SHADOWS;
	}

	Surface& Platform::getSurface() const {
		return *_surface;
	}

	SurfaceUI& Platform::getSurfaceUI() const {
		return *_surfaceUI;
	}

	SurfaceGame& Platform::getSurfaceGame() const {
		return *_surfaceGame;
	}

	SurfaceGame* Platform::getSurfaceGameShadows() const {
		return _surfaceGameShadows.get();
	}
}
