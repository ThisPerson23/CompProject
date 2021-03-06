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
		, death_(textures.get(TextureID::ZombieDeath))
		, sprite_(textures.get(TABLE.at(type).texture))
		, animations_()
		, travelDistance_(0.f)
		, directionIndex_(0)
		, attackCommand_()
		, spawnPickup_(false)
		, dropPickupCommand_()
		, showDeath_(false)
		, hasPlayedDeathSound_(false)
		, attackInterval_(sf::Time::Zero)
		, attackClock_()
	{
		setupAnimations();

		//Set up drop pickup command
		dropPickupCommand_.category = Category::GroundLayer;
		dropPickupCommand_.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			createPickup(node, textures);
		};
	}

	void Zombie::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		switch (state_)
		{
		case Zombie::State::Up:
			target.draw(walkUp_, states);
			break;
		case Zombie::State::Left:
			target.draw(walkLeft_, states);
			break;
		case Zombie::State::Down:
			target.draw(walkDown_, states);
			break;
		case Zombie::State::Right:
			target.draw(walkRight_, states);
			break;
		case Zombie::State::Dead:
			target.draw(death_, states);
			break;
		default:
			//Do Nothing
			break;
		}
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
		return isDestroyed() && death_.isFinished();
	}

	void Zombie::remove()
	{
		Entity::remove();
		showDeath_ = false;
	}

	Zombie::ZombieType Zombie::getType() const
	{
		return type_;
	}

	int Zombie::getDamage() const
	{
		return TABLE.at(type_).damage;
	}

	sf::Time Zombie::getAttackInterval() const
	{
		return attackInterval_;
	}

	void Zombie::setAttackInterval(sf::Time interval)
	{
		attackInterval_ = interval;
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

	sf::Clock Zombie::getAttackClock() const
	{
		return attackClock_;
	}

	void Zombie::setAttackClock(sf::Clock clock)
	{
		attackClock_ = clock;
	}

	sf::Time Zombie::getAttackDelay() const
	{
		return TABLE.at(type_).attackInterval;
	}

	void Zombie::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//Update the states
		updateStates(dt);

		switch (state_)
		{
			case Zombie::State::Up:
				walkUp_.update(dt);
				break;
			case Zombie::State::Left:
				walkLeft_.update(dt);
				break;
			case Zombie::State::Down:
				walkDown_.update(dt);
				break;
			case Zombie::State::Right:
				walkRight_.update(dt);
				break;
			case Zombie::State::Dead:
				death_.update(dt);
			default:
				//Do Nothing
				break;
		}

		Entity::updateCurrent(dt, commands);

		if (isDestroyed() && state_ == Zombie::State::Dead)
		{
			//start death animation
			death_.update(dt);

			//Play Death Sound
			if (!hasPlayedDeathSound_)
			{
				hasPlayedDeathSound_ = true;
				SoundEffectID effect = SoundEffectID::ZombieDeath;
				playLocalSound(commands, effect);
			}
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
		auto type = static_cast<Pickup::Type>(randomInt(static_cast<int>(Pickup::Type::Count)));

		std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
		pickup->setPosition(getWorldPosition());
		pickup->setVelocity(0.f, 0.f);
		node.attachChild(std::move(pickup));
	}

	void Zombie::checkPickupDrop(CommandQueue & commands)
	{
		if (randomInt(3) == 0 && !spawnPickup_)
			commands.push(dropPickupCommand_);

		spawnPickup_ = true;
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

		death_.setFrameSize(sf::Vector2i(40, 45));
		death_.setNumFrames(3);
		death_.setDuration(sf::seconds(1.5f));

		animations_[Zombie::State::Up] = walkUp_;
		animations_[Zombie::State::Left] = walkLeft_;
		animations_[Zombie::State::Down] = walkDown_;
		animations_[Zombie::State::Right] = walkRight_;
		animations_[Zombie::State::Dead] = death_;

		centerOrigin(walkUp_);
		centerOrigin(walkLeft_);
		centerOrigin(walkDown_);
		centerOrigin(walkRight_);
		centerOrigin(death_);
	}
}