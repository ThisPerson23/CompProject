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

#pragma once

#include "SceneNode.h"
#include "CommandQueue.h"

namespace GEX
{ 
	class Entity : public SceneNode
	{
	public:
							Entity(int hp);

		void				setVelocity(sf::Vector2f velocity);
		void				setVelocity(float vx, float vy);

		sf::Vector2f		getVelocity() const;

		void				accelerate(sf::Vector2f velocity);
		void				accelerate(float vx, float vy);

		bool				isDestroyed() const override;

		void				damage(int points);
		void				repair(int points);
		void				destroy();
		virtual void		remove();

		int					getHitpoints() const;

	protected:
		 void				updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Vector2f		velocity_;
		int					hitPoints_;
	};
}
