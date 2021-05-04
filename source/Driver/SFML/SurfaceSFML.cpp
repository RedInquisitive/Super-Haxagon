#include "Driver/SFML/SurfaceSFML.hpp"

namespace SuperHaxagon {
	SurfaceSFML::SurfaceSFML(sf::VideoMode mode) {
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;

		_window = std::make_unique<sf::RenderWindow>(mode, "Super Haxagon", sf::Style::Default, settings);
		_window->setVerticalSyncEnabled(true);
	}

	void SurfaceSFML::drawPolyAbsolute(const Color& color, const std::vector<Vec2f>& points) {
		const sf::Color sfColor{ color.r, color.g, color.b, color.a };
		sf::ConvexShape convex(points.size());
		convex.setPosition(0, 0);
		convex.setFillColor(sfColor);
		auto index = 0;
		for (const auto& point : points) {
			convex.setPoint(index++, sf::Vector2f(point.x, point.y));
		}

		_window->draw(convex);
	}

	Vec2f SurfaceSFML::getScreenDim() const {
		Vec2f point{};
		point.x = static_cast<float>(_window->getSize().x);
		point.y = static_cast<float>(_window->getSize().y);
		return point;
	}

	void SurfaceSFML::screenBegin() {
		_window->clear(sf::Color::Black);
	}

	void SurfaceSFML::screenFinalize() {
		_window->display();
	}

	sf::RenderWindow* SurfaceSFML::getWindow() const {
		return _window.get();
	}
}
