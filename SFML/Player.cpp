/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* Player class
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

#include "Player.h"
#include "DataTables.h"
#include "Utility.h"
#include "Category.h"
#include "TextNode.h"
#include "SoundNode.h"
#include "CommandQueue.h"

#include <string>

using namespace std::placeholders;

namespace GEX
{ 
	namespace
	{
		const std::map<Player::Type, PlayerData> TABLE = initializePlayerData();
	}
	
	Player::Player(Player::Type type, const TextureManager& textures)
		: Entity(TABLE.at(type).hitpoints)
		, type_(type)
		, sprite_(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect)
		, death_(textures.get(TextureID::PlayerDeath))
		, walkUp_(textures.get(TextureID::PlayerWalkUp))
		, walkLeft_(textures.get(TextureID::PlayerWalkLeft))
		, walkDown_(textures.get(TextureID::PlayerWalkDown))
		, walkRight_(textures.get(TextureID::PlayerWalkRight))
		, idleUp_(textures.get(TextureID::PlayerIdleUp))
		, idleLeft_(textures.get(TextureID::PlayerIdleLeft))
		, idleDown_(textures.get(TextureID::PlayerIdleDown))
		, idleRight_(textures.get(TextureID::PlayerIdleRight))
		, showDeath_(true)
		, healthDisplay_(nullptr)
		, ammoDisplay_(nullptr)
		, isFiring_(false)
		, isMarkedForRemoval_(false)
		, ammo_(250)
		, fireCountDown_(sf::Time::Zero)
		, fireCommand_()
		, state_(Player::State::IdleDown)
	{
		setupAnimations();

		centerOrigin(idleUp_);
		centerOrigin(idleLeft_);
		centerOrigin(idleDown_);
		centerOrigin(idleRight_);

		//Set up Commands
		fireCommand_.category = Category::GroundLayer;
		fireCommand_.action = [this, &textures] (SceneNode& node, sf::Time dt) 
		{
			createBullets(node, textures);
		};

		//set up text for health and missiles
		std::unique_ptr<TextNode> health(new TextNode(""));
		healthDisplay_ = health.get();
		attachChild(std::move(health));

		std::unique_ptr<TextNode> ammoDisplay(new TextNode(""));
		ammoDisplay->setPosition(0, 70);
		ammoDisplay_ = ammoDisplay.get();
		attachChild(std::move(ammoDisplay));

		updateTexts();
		
	}

	unsigned int Player::getCategory() const
	{
		return Category::Player;
	}

	void Player::updateTexts()
	{
		// Display hitpoints
		if (isDestroyed())
			healthDisplay_->setString("");
		else
			healthDisplay_->setString("HP " + std::to_string(getHitpoints()));

		healthDisplay_->setPosition(0.f, 50.f);
		healthDisplay_->setRotation(-getRotation());

		// Display ammo
		if (ammoDisplay_)
		{
			if (ammo_ == 0 || isDestroyed())
				ammoDisplay_->setString("");
			else
				ammoDisplay_->setString("Ammo " + std::to_string(ammo_));
		}
	}

	void Player::fire()
	{
		if (TABLE.at(type_).fireInterval != sf::Time::Zero)
			isFiring_ = true;
	}

	void Player::collectAmmo(unsigned int count)
	{
		ammo_ += count;
	}

	sf::FloatRect Player::getBoundingBox() const
	{
		auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());

		box.top += 12;
		box.left += 12;
		box.width -= 20;
		box.height -= 20;

		return box;
	}

	bool Player::isMarkedForRemoval() const
	{
		return isDestroyed() && (death_.isFinished() || !showDeath_);
	}

	void Player::remove()
	{
		Entity::remove();
		showDeath_ = false;
	}

	void Player::playLocalSound(CommandQueue& commands, SoundEffectID effect)
	{
		Command playSoundCommand;
		playSoundCommand.category = Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, _1, effect, getWorldPosition())
			);

		commands.push(playSoundCommand);
	}

	void Player::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		//Check if bullets or missiles are fired
		checkProjectileLaunch(dt, commands);

		updateStates(dt);

		switch (state_)
		{
			case Player::State::Dead:
				death_.update(dt);
				break;
			case Player::State::WalkDown:
				walkDown_.update(dt);
				break;
			case Player::State::WalkUp:
				walkUp_.update(dt);
				break;
			case Player::State::WalkLeft:
				walkLeft_.update(dt);
				break;
			case Player::State::WalkRight:
				walkRight_.update(dt);
				break;
			default:
				//Do Nothing
				break;
		}

		Entity::updateCurrent(dt, commands);

		if (!isDestroyed())
		{
			updateTexts();
		}
	}

	void Player::updateStates(sf::Time dt)
	{
		//Check if player is dead
		if (isDestroyed())
			setState(Player::State::Dead);

		//If player is not dead and is not moving, check the last state they were in and set the appropriate idle state
		if (state_ != Player::State::Dead)
		{ 
			if (getVelocity().x == 0 && getVelocity().y == 0)
			{
				switch (state_)
				{
					case Player::State::WalkUp:
						setState(Player::State::IdleUp);
						break;
					case Player::State::WalkRight:
						setState(Player::State::IdleRight);
						break;
					case Player::State::WalkLeft:
						setState(Player::State::IdleLeft);
						break;
					case Player::State::WalkDown:
						setState(Player::State::IdleDown);
						break;
					default:
						//Do Nothing
						break;
				}
			}
			//If player is moving, set the appropriate walking state
			else
			{
				if (getVelocity().y != 0)
				{
					if (getVelocity().y < 0)
						setState(Player::State::WalkUp);
					else
						setState(Player::State::WalkDown);
				}
				else
				{
					if (getVelocity().x < 0)
						setState(Player::State::WalkLeft);
					else
						setState(Player::State::WalkRight);
				}
			}
		}
	}

	void Player::setState(Player::State state)
	{
		state_ = state;
	}

	Player::State Player::getState() const
	{
		return state_;
	}

	float Player::getMaxSpeed() const
	{
		return TABLE.at(type_).speed;
	}

	void Player::createBullets(SceneNode & node, const TextureManager & textures)
	{
		Projectile::Type type = Projectile::Type::AlliedBullet;

		switch (state_)
		{
			case Player::State::WalkUp:
			case Player::State::IdleUp:
				createProjectile(node, type, 0.f, 0.5f, textures);
				break;
			case Player::State::WalkLeft:
			case Player::State::IdleLeft:
				createProjectile(node, type, 0.5f, 0.f, textures);
				break;
			case Player::State::WalkDown:
			case Player::State::IdleDown:
				createProjectile(node, type, 0.f, -0.5f, textures);
				break;
			case Player::State::WalkRight:
			case Player::State::IdleRight:
				createProjectile(node, type, -0.5f, 0.f, textures);
				break;
			default:
				break;
		}
	}

	void Player::createProjectile(SceneNode & node, Projectile::Type type, float xoffset, float yoffset, const TextureManager & textures)
	{
		std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
		sf::Vector2f velocity;

		sf::Vector2f offset;

		switch (state_)
		{
			case Player::State::WalkUp:
			case Player::State::IdleUp:
				velocity.x = 0;
				velocity.y = projectile->getMaxSpeed() * 1.f;
				break;
			case Player::State::WalkLeft:
			case Player::State::IdleLeft:
				velocity.x = projectile->getMaxSpeed() * 1.f;
				velocity.y = 0;
				projectile->setRotation(90.f);
				break;
			case Player::State::WalkDown:
			case Player::State::IdleDown:
				velocity.x = 0;
				velocity.y = projectile->getMaxSpeed() * -1.f;
				break;
			case Player::State::WalkRight:
			case Player::State::IdleRight:
				velocity.x = projectile->getMaxSpeed() * -1.f;
				velocity.y = 0;
				projectile->setRotation(90.f);
				break;
			default:
				//Do Nothing
				break;
		}
		float sign = -1.f;

		projectile->setPosition(getWorldPosition() + offset * sign);
		projectile->setVelocity(velocity * sign);
		node.attachChild(std::move(projectile));
	}

	void Player::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
	{

		//Bullets
		if (isFiring_ && fireCountDown_ <= sf::Time::Zero)
		{
			//Can only fire if player has ammo
			if (ammo_ > 0)
			{ 
				commands.push(fireCommand_);
				playLocalSound(commands, SoundEffectID::PistolShot);
				isFiring_ = false;
				--ammo_;
				fireCountDown_ = TABLE.at(type_).fireInterval;
			}
		}
		else if (fireCountDown_ > sf::Time::Zero)
		{
			fireCountDown_ -= dt;
		}
	}

	void Player::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		switch (state_)
		{
			case Player::State::Dead:
				target.draw(death_, states);
				break;
			case Player::State::WalkRight:
				target.draw(walkRight_, states);
				break;
			case Player::State::WalkLeft:
				target.draw(walkLeft_, states);
				break;
			case Player::State::WalkDown:
				target.draw(walkDown_, states);
				break;
			case Player::State::WalkUp:
				target.draw(walkUp_, states);
				break;
			case Player::State::IdleUp:
				target.draw(idleUp_, states);
				break;
			case Player::State::IdleLeft:
				target.draw(idleLeft_, states);
				break;
			case Player::State::IdleDown:
				target.draw(idleDown_, states);
				break;
			case Player::State::IdleRight:
				target.draw(idleRight_, states);
				break;
			default:
				//Do Nothing
				target.draw(sprite_, states);
				break;
		}
	}

	void Player::setupAnimations() 
	{
		//Death
		death_.setFrameSize(sf::Vector2i(35, 39));
		death_.setNumFrames(4);
		death_.setDuration(sf::seconds(1));

		//Walk Up
		walkUp_.setFrameSize(sf::Vector2i(27, 39));
		walkUp_.setNumFrames(7);
		walkUp_.setDuration(sf::seconds(1.5));
		walkUp_.setRepeating(true);

		//Walk Left
		walkLeft_.setFrameSize(sf::Vector2i(30, 39));
		walkLeft_.setNumFrames(7);
		walkLeft_.setDuration(sf::seconds(1.5));
		walkLeft_.setRepeating(true);

		//Walk Down
		walkDown_.setFrameSize(sf::Vector2i(26, 39));
		walkDown_.setNumFrames(7);
		walkDown_.setDuration(sf::seconds(1.5));
		walkDown_.setRepeating(true);

		//Walk Right
		walkRight_.setFrameSize(sf::Vector2i(30, 39));
		walkRight_.setNumFrames(7);
		walkRight_.setDuration(sf::seconds(1.5));
		walkRight_.setRepeating(true);

		centerOrigin(death_);
		centerOrigin(sprite_);
		centerOrigin(walkUp_);
		centerOrigin(walkLeft_);
		centerOrigin(walkDown_);
		centerOrigin(walkRight_);
	}
}