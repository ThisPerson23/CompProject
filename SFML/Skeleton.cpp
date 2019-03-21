/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* Skeleton Class
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

#include "Skeleton.h"
#include "DataTables.h"
#include "Utility.h"
#include "SoundNode.h"

using namespace std::placeholders;

namespace GEX
{ 
	namespace
	{
		const std::map<Skeleton::SkeletonType, SkeletonData> TABLE = initializeSkeletonData();
	}

	Skeleton::Skeleton(Skeleton::SkeletonType type, const TextureManager& textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, state_(Skeleton::State::Down)
		, walkUp_(textures.get(TextureID::SkeletonWalkUp))
		, walkLeft_(textures.get(TextureID::SkeletonWalkUp))
		, walkDown_(textures.get(TextureID::SkeletonWalkDown))
		, walkRight_(textures.get(TextureID::SkeletonWalkRight))
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

	void Skeleton::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(sprite_, states);
	}

	unsigned int Skeleton::getCategory() const
	{
		return Category::Skeleton;
	}

	sf::FloatRect Skeleton::getBoundingBox() const
	{
		return getWorldTransform().transformRect(sprite_.getGlobalBounds());
	}

	bool Skeleton::isMarkedForRemoval() const
	{
		return isDestroyed() && state_ == Skeleton::State::Dead;
	}

	void Skeleton::remove()
	{
		Entity::remove();
		showDeath_ = false;
	}

	void Skeleton::playLocalSound(CommandQueue & commands, SoundEffectID effect)
	{
		Command playSoundCommand;
		playSoundCommand.category = Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, _1, effect, getWorldPosition())
			);

		commands.push(playSoundCommand);
	}

	void Skeleton::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		//Update the states
		updateStates(dt);

		if (isDestroyed() && state_ == Skeleton::State::Dead)
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

	float Skeleton::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	void Skeleton::updateStates(sf::Time dt)
	{
		if (isDestroyed())
			state_ = Skeleton::State::Dead;
		else
		{
			if (getVelocity().y != 0)
			{
				if (getVelocity().y < 0)
					state_ = Skeleton::State::Up;
				else
					state_ = Skeleton::State::Down;
			}
			else
			{
				if (getVelocity().x < 0)
					state_ = Skeleton::State::Left;
				else
					state_ = Skeleton::State::Right;
			}
		}
	}

	void Skeleton::setState(Skeleton::State state)
	{
		state_ = state;
		animations_[state_].restart();
	}

	Skeleton::State Skeleton::getState() const
	{
		return state_;
	}

	void Skeleton::createPickup(SceneNode & node, const TextureManager & textures) const
	{
	}

	void Skeleton::checkPickupDrop(CommandQueue & commands)
	{
	}

	void Skeleton::setupAnimations()
	{
		walkUp_.setFrameSize(sf::Vector2i(33, 15));
		walkUp_.setNumFrames(3);
		walkUp_.setDuration(sf::milliseconds(500));

		walkLeft_.setFrameSize(sf::Vector2i(33, 15));
		walkLeft_.setNumFrames(3);
		walkLeft_.setDuration(sf::milliseconds(500));

		walkDown_.setFrameSize(sf::Vector2i(33, 15));
		walkDown_.setNumFrames(3);
		walkDown_.setDuration(sf::milliseconds(500));

		walkRight_.setFrameSize(sf::Vector2i(33, 15));
		walkRight_.setNumFrames(3);
		walkRight_.setDuration(sf::milliseconds(500));

		animations_[Skeleton::State::Up] = walkUp_;
		animations_[Skeleton::State::Left] = walkLeft_;
		animations_[Skeleton::State::Down] = walkDown_;
		animations_[Skeleton::State::Right] = walkRight_;

		centerOrigin(walkUp_);
		centerOrigin(walkLeft_);
		centerOrigin(walkDown_);
		centerOrigin(walkRight_);
	}
}