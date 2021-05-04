#ifndef SUPER_HAXAGON_SURFACESFML_HPP
#define SUPER_HAXAGON_SURFACESFML_HPP

#include "Core/Surface.hpp"

#include <SFML/Graphics.hpp>

namespace SuperHaxagon {
	class SurfaceSFML : public Surface {
	public:
		explicit SurfaceSFML(sf::VideoMode mode);
		virtual ~SurfaceSFML() = default;

		virtual void drawPolyAbsolute(const Color& color, const std::vector<Vec2f>& points) override;

		virtual Vec2f getScreenDim() const override;
		virtual void screenBegin() override;
		virtual void screenFinalize() override;

		sf::RenderWindow* getWindow() const;

	private:
		std::unique_ptr<sf::RenderWindow> _window;
	};
}

#endif //SUPER_HAXAGON_SURFACESFML_HPP
