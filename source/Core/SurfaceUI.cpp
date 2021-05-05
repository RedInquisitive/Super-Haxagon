#include "Core/SurfaceUI.hpp"

#include "Core/Surface.hpp"

namespace SuperHaxagon {
	SurfaceUI::SurfaceUI(Surface* surface) : _surface(surface) {}

	void SurfaceUI::drawPolyUI(const Color& color, const std::vector<Vec2f>& points) {
		if (!_surface) return;

		// Passthrough to underlying surface
		_surface->drawPolyAbsolute(color, points);
	}

	void SurfaceUI::drawRectUI(const Color color, const Vec2f position, const Vec2f size) {
		const std::vector<Vec2f> points{
				{position.x, position.y + size.y},
				{position.x + size.x, position.y + size.y},
				{position.x + size.x, position.y},
				{position.x, position.y},
		};

		this->drawPolyUI(color, points);
	}

	Vec2f SurfaceUI::getScreenDim() const {
		if (!_surface) return {};
		return _surface->getScreenDim();
	}

	float SurfaceUI::getScale() const {
		// The original game was built with a 3DS in mind, so when
		// drawing we have to scale the game to however many times larger the viewport is.
		return this->getScreenDimMin() / 240.0f;
	}

	float SurfaceUI::getScreenDimMax() const {
		const auto size = this->getScreenDim();
		return std::max(size.x, size.y);
	}

	float SurfaceUI::getScreenDimMin() const {
		const auto size = this->getScreenDim();
		return std::min(size.x, size.y);
	}
}