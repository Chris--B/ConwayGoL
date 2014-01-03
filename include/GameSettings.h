#pragma once

#include <string>

#include <SFML/Graphics.hpp> // for graphic settings

namespace Conway {
	class GameSettings {
	public:
		static GameSettings fromFile(const std::string& filename);
		void saveToFile(const std::string& filename);
		
		bool fullscreen;
		bool start_paused;

		int resolution_width;
		int resolution_height;
		int speed;
		int grid_width;
		int grid_height;

		sf::Color background_color;
		sf::Color cell_color;

		std::string window_title;
	};
} // namespace Conway