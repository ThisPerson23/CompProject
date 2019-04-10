/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Aircraft Class
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

#include <SFML\Graphics\Sprite.hpp>

#include "Entity.h"
#include "Command.h"
#include "ResourceIdentifiers.h"
#include "TextureManager.h"
#include "Projectile.h"
#include "TextNode.h"
#include "Animation.h"

namespace GEX
{
	class Player : public Entity
	{
	public:
		enum class Type
		{
			Player
		};

		enum class State
		{
			WalkUp,
			WalkLeft,
			WalkDown,
			WalkRight,
			IdleUp,
			IdleLeft,
			IdleDown,
			IdleRight,
			Dead
		};

	public:
								Player(Player::Type type, const TextureManager& textures);
		
		void					drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int			getCategory() const override;

		//bool					isAllied() const;

		void					fire();
		//void					launchMissile();

		//void					increaseFireRate();
		//void					increaseFireSpread();
		void					collectAmmo(unsigned int count);
		sf::FloatRect			getBoundingBox() const override;

		bool					isMarkedForRemoval() const override;

		void					remove() override;

		void					playLocalSound(CommandQueue& commands, SoundEffectID effect);

	protected:
		void					updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		//void					updateMovementPattern(sf::Time dt);
		float					getMaxSpeed() const;

		void					setupAnimations();

		void					updateStates(sf::Time dt);

		void					setState(Player::State state);
		Player::State			getState() const;

		void					createBullets(SceneNode& node, const TextureManager& textures);
		void					createProjectile(SceneNode& node, Projectile::Type type, 
												 float xoffset, float yoffset, const TextureManager& textures);
		//void					createPickup(SceneNode& node, const TextureManager& textures) const;

		//void					checkPickupDrop(CommandQueue& commands);
		void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

		void					updateTexts(); //update mini health and missile display
		//void					updateRollAnimation(); //update the sprite of the player aircraft, depending on the travel direction

	private:
		Type					type_;
		sf::Sprite				sprite_;
		Animation				death_;
		bool					showDeath_;

		Animation				walkUp_;
		Animation				walkLeft_;
		Animation				walkDown_;
		Animation				walkRight_;

		sf::Sprite				idleUp_;
		sf::Sprite				idleLeft_;
		sf::Sprite				idleDown_;
		sf::Sprite				idleRight_;

		TextNode*				healthDisplay_;
		TextNode*				ammoDisplay_;

		//float					travelDistance_;
		//std::size_t				directionIndex_;

		bool					isFiring_;
		//bool					isLaunchingMissiles_;
		bool					isMarkedForRemoval_;

		//int						fireRateLevel_;
		//int						fireSpreadLevel_;
		int						ammo_;

		sf::Time				fireCountDown_;

		Command					fireCommand_;
		//Command					launchMissileCommand_;
		//Command					dropPickupCommand_;

		//bool					spawnPickup_;

		bool					hasPlayedDeathSound_;

		Player::State			state_;
	};
}
