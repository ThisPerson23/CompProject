/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Entity Class
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

#include "Entity.h"

namespace GEX
{
	Entity::Entity(int hp)
	{
		hitPoints_ = hp;
	}
	void Entity::setVelocity(sf::Vector2f velocity)
	{
		velocity_ = velocity;
	}

	void Entity::setVelocity(float vx, float vy)
	{
		velocity_.x = vx;
		velocity_.y = vy;
	}

	sf::Vector2f Entity::getVelocity() const
	{
		return velocity_;
	}

	void Entity::accelerate(sf::Vector2f velocity)
	{
		velocity_ += velocity;
	}

	void Entity::accelerate(float vx, float vy)
	{
		velocity_.x += vx;
		velocity_.y += vy;
	}

	bool Entity::isDestroyed() const
	{
		return hitPoints_ <= 0;
	}

	void Entity::damage(int points)
	{
		assert(points > 0);
		hitPoints_ -= points;
	}

	void Entity::repair(int points)
	{
		assert(points > 0);
		hitPoints_ += points;
	}

	void Entity::destroy()
	{
		hitPoints_ = 0;
	}

	void Entity::remove()
	{
		destroy();
	}

	int Entity::getHitpoints() const
	{
		return hitPoints_;
	}

	void Entity::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		move(velocity_ * dt.asSeconds());
	}
	void Entity::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
	}
}