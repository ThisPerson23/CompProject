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

#include "Zombie.h"
#include "TextureManager.h"
#include "DataTables.h"
#include "Utility.h"
#include "SoundNode.h"
#include "Command.h"

using namespace std::placeholders;

namespace GEX
{ 
	namespace
	{
		const std::map<Zombie::ZombieType, ZombieData> TABLE = initializeZombieData();
	}

	Zombie::Zombie(Zombie::ZombieType type, const TextureManager& textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, state_()
		, walkUp_(textures.get(TextureID::ZombieWalkUp))
		, walkLeft_(textures.get(TextureID::ZombieWalkLeft))
		, walkDown_(textures.get(TextureID::ZombieWalkDown))
		, walkRight_(textures.get(TextureID::ZombieWalkRight))
		, sprite_(textures.get(TABLE.at(type).texture))
		, animations_()
		, travelDistance_(0.f)
		, directionIndex_(0)
		, attackCommand_()
		, spawnPickup_(false)
		, showDeath_(false)
		, hasPlayedDeathSound_(false)
	{
		setupAnimations();
	}

	void Zombie::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		/*target.draw(sprite_, states);*/

		if (getVelocity().x > 0)
			target.draw(walkRight_, states);
		else if (getVelocity().x < 0)
			target.draw(walkLeft_, states);
		else if (getVelocity().y > 0)
			target.draw(walkDown_, states);
		else if (getVelocity().y < 0)
			target.draw(walkUp_, states);
		else if (getVelocity().x == 0 && getVelocity().y == 0)
			target.draw(sprite_, states);
	}

	unsigned int Zombie::getCategory() const
	{
		return Category::Zombie;
	}

	sf::FloatRect Zombie::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());

		box.left -= 15;
		box.top -= 20;

		return box;
	}

	bool Zombie::isMarkedForRemoval() const
	{
		return isDestroyed();/* && state_ == Zombie::State::Dead;*/
	}

	void Zombie::remove()
	{
		Entity::remove();
		showDeath_ = false;
	}

	int Zombie::getDamage() const
	{
		return TABLE.at(type_).damage;
	}

	void Zombie::playLocalSound(CommandQueue & commands, SoundEffectID effect)
	{
		Command playSoundCommand;
		playSoundCommand.category = Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, _1, effect, getWorldPosition())
			);

		commands.push(playSoundCommand);
	}

	void Zombie::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//Update the states
		updateStates(dt);

		if (getVelocity().x > 0)
			walkRight_.update(dt);
		else if (getVelocity().x < 0)
			walkLeft_.update(dt);
		else if (getVelocity().y > 0)
			walkDown_.update(dt);
		else if (getVelocity().y < 0)
			walkUp_.update(dt);
		/*else if (getVelocity().x == 0 && getVelocity().y == 0)
			target.draw(sprite_, states);*/

		//animations_[state_].update(dt);

		Entity::updateCurrent(dt, commands);

		if (isDestroyed() && state_ == Zombie::State::Dead)
		{
			//Start death animation
			//death_.update(dt);

			//Play Death Sound
			/*if (!hasPlayedDeathSound_)
			{
				hasPlayedDeathSound_ = true;
				SoundEffectID effect = SoundEffectID::ZombieDeath;
				playLocalSound(commands, effect);
			}*/
		}

	}

	void Zombie::updateStates(sf::Time dt)
	{
		if (isDestroyed())
			setState(Zombie::State::Dead);
		else
		{
			if (getVelocity().y != 0)
			{
				if (getVelocity().y < 0)
					setState(Zombie::State::Up);
				else
					setState(Zombie::State::Down);
			}
			else
			{
				if (getVelocity().x < 0)
					setState(Zombie::State::Left);
				else
					setState(Zombie::State::Right);
			}
		}


	}

	float Zombie::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	void Zombie::setState(Zombie::State state)
	{
		state_ = state;
		animations_[state_].restart();
	}
	Zombie::State Zombie::getState() const
	{
		return state_;
	}

	void Zombie::createPickup(SceneNode & node, const TextureManager & textures) const
	{
	}

	void Zombie::checkPickupDrop(CommandQueue & commands)
	{
	}

	void Zombie::setupAnimations()
	{
		walkUp_.setFrameSize(sf::Vector2i(33, 45));
		walkUp_.setNumFrames(3);
		walkUp_.setDuration(sf::seconds(0.5));
		walkUp_.setRepeating(true);

		walkLeft_.setFrameSize(sf::Vector2i(33, 45));
		walkLeft_.setNumFrames(3);
		walkLeft_.setDuration(sf::seconds(0.5));
		walkLeft_.setRepeating(true);

		walkDown_.setFrameSize(sf::Vector2i(33, 45));
		walkDown_.setNumFrames(3);
		walkDown_.setDuration(sf::seconds(0.5));
		walkDown_.setRepeating(true);

		walkRight_.setFrameSize(sf::Vector2i(33, 45));
		walkRight_.setNumFrames(3);
		walkRight_.setDuration(sf::seconds(0.5));
		walkRight_.setRepeating(true);

		animations_[Zombie::State::Up] = walkUp_;
		animations_[Zombie::State::Left] = walkLeft_;
		animations_[Zombie::State::Down] = walkDown_;
		animations_[Zombie::State::Right] = walkRight_;

		centerOrigin(walkUp_);
		centerOrigin(walkLeft_);
		centerOrigin(walkDown_);
		centerOrigin(walkRight_);
	}
}