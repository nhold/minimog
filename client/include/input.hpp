#pragma once

#include<map>
#include <SFML/Window/Event.hpp>

class Input
{
public:
	void Reset()
	{
		for (auto & [key, val] : justReleased)
		{
			val = false;
		}
	}

	void Process(sf::Event windowEvent)
	{
		if (windowEvent.type == sf::Event::KeyReleased)
		{	
			justReleased[windowEvent.key.code] = true;
		}

		if (windowEvent.type == sf::Event::GainedFocus)
		{
			hasFocus = true;
		}

		if (windowEvent.type == sf::Event::LostFocus)
		{
			hasFocus = false;
		}
	}

	std::map<sf::Keyboard::Key, bool> justReleased;
	bool hasFocus = true;
};