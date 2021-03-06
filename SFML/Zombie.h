/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* Zombie Class
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

#include "Animation.h"
#include "Entity.h"
#include "TextureManager.h"

namespace GEX
{
	class Zombie : public Entity
	{
	public:
		enum class ZombieType
		{
			Zombie
		};

		enum class State
		{
			Up,
			Down,
			Left,
			Right,
			Idle,
			Dead
		};

	public:
								Zombie(Zombie::ZombieType type, const TextureManager& textures);

		void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int			getCategory() const override;

		sf::FloatRect			getBoundingBox() const override;

		bool					isMarkedForRemoval() const override;

		void					remove() override;

		Zombie::ZombieType		getType() const;
		Zombie::State			getState() const;

		void					playLocalSound(CommandQueue& commands, SoundEffectID effect);

		float					getMaxSpeed() const;
		int						getDamage() const;

		sf::Time				getAttackInterval() const;
		void					setAttackInterval(sf::Time interval);

		sf::Clock				getAttackClock() const;
		void					setAttackClock(sf::Clock clock);

		sf::Time				getAttackDelay() const;

	protected:
		void					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		
		void					updateStates(sf::Time dt);

		void					setState(Zombie::State state);

		void					createPickup(SceneNode& node, const TextureManager& textures) const;
		void					checkPickupDrop(CommandQueue& commands);

		void					setupAnimations();

	private:
		Zombie::ZombieType				   type_;
		Zombie::State					   state_;

		Animation						   walkUp_;
		Animation						   walkLeft_;
		Animation						   walkDown_;
		Animation						   walkRight_;
		Animation						   death_;

		sf::Sprite						   sprite_;

		std::map<Zombie::State, Animation> animations_;

		float							   travelDistance_;
		std::size_t						   directionIndex_;

		sf::Time						   attackInterval_;
		sf::Clock						   attackClock_;

		Command							   attackCommand_;

		bool							   spawnPickup_;

		Command							   dropPickupCommand_;
										   
		bool							   showDeath_;
		bool							   hasPlayedDeathSound_;
	};
}
