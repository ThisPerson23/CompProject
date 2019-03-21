/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Utility Class
*
*
*
*
* @section LICENSE
*
*
* Copyright 2018
* Permission to use, copy, modify, and/or distribute this software for
* any purpose with or without fee is hereby granted, provided that the
* above copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
* @section Academic Integrity
* I certify that this work is solely my own and complies with
* NBCC Academic Integrity Policy (policy 1111)
*/

#include "Utility.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>

#include <random>

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#ifndef M_PI
#define M_PI (3.141592)
#endif

namespace GEX
{ 
	void centerOrigin(sf::Sprite & sprite)
	{
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}

	void centerOrigin(sf::Text & text)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}

	void centerOrigin(Animation & animation)
	{
		sf::FloatRect bounds = animation.getLocalBounds();
		animation.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}

	float toDegree(float radian)
	{
		return 180.f / static_cast<float>(M_PI) * radian;
	}

	float toRadian(float degree)
	{
		return static_cast<float>(M_PI) / 180.f * degree;
	}

	int randomInt(int exclusiveMax)
	{
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(0, exclusiveMax - 1);

		return uni(rng);
	}

	float length(sf::Vector2f vector)
	{
		return std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
	}

	sf::Vector2f unitVector(sf::Vector2f vector)
	{
		assert(vector != sf::Vector2f(0.f, 0.f));
		return vector / length(vector);
	}
}