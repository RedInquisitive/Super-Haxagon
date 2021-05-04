#ifndef SUPER_HAXAGON_FONT_SFML_HPP
#define SUPER_HAXAGON_FONT_SFML_HPP

#include "Core/Font.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

namespace SuperHaxagon {
	class SurfaceSFML;
	class FontSFML : public Font {
	public:
		FontSFML(SurfaceSFML* surface, const std::string& path, float size);
		~FontSFML() override;

		void setScale(float scale) override;
		float getHeight() const override;
		float getWidth(const std::string& text) const override;
		void draw(const Color& color, const Vec2f& position, Alignment alignment, const std::string& text) override;

	private:
		SurfaceSFML* _surface;
		sf::Font _font;
		bool _loaded = false;
		float _scale;
		float _size;
	};
}

#endif //SUPER_HAXAGON_FONT_SFML_HPP
