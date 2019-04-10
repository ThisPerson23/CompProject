/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* World Class
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

#include "World.h"
#include "Player.h"
#include "Pickup.h"
#include "Utility.h"
#include "Projectile.h"
#include "SoundNode.h"
#include "ParticleNode.h"

namespace GEX
{ 
	World::World(sf::RenderWindow& window, SoundPlayer& sounds)
	: target_(window)
	, sounds_(sounds)
	, worldView_(window.getDefaultView())
	, textures_()
	, sceneGraph_()
	, sceneLayers_()
	, worldBounds_(0.f, 0.f, worldView_.getSize().x, /*5000.f*/worldView_.getSize().y)
	, spawnPosition_(worldView_.getSize().x / 2.f, worldBounds_.height - worldView_.getSize().y / 2.f)
	, scrollSpeed_(0.f)
	, player_(nullptr)
	, scoreText_()
	, score_()
	, multiplierText_()
	, multiplier_()
	, enemySpawnDelay_(sf::seconds(5))
	, enemySpawnTimer_(sf::Time::Zero)
	, enemySpawnClock_()
	{
		//Font
		scoreText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Spooky));
		scoreText_.setPosition(worldView_.getSize().x / 2.f - 50.f, 20.f);
		scoreText_.setCharacterSize(25);
		scoreText_.setString("Score " + std::to_string(score_));

		multiplierText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Spooky));
		multiplierText_.setPosition(worldView_.getSize().x / 2.f - 50.f, 40.f);
		multiplierText_.setCharacterSize(25);
		multiplierText_.setString("X" + std::to_string(multiplier_));

		loadTextures();

		//prepare the view
		/*worldView_.setCenter(spawnPosition_);*/

		buildScene();
	}

	void World::update(sf::Time dt, CommandQueue& commands)
	{
		//Setup enemy spawn points
		setupSpawnPoints();

		// Scroll screen and reset player velocity
		worldView_.move(0.f, scrollSpeed_ * dt.asSeconds());
		player_->setVelocity(0.f, 0.f);

		// Destroy all entities that leave the battlefield
		destroyEntitiesOutOfView();

		// Guide missiles
		guideMissiles();

		// Run all the commands in the command queue
		while (!commandQueue_.isEmpty())
		{ 
			sceneGraph_.onCommand(commandQueue_.pop(), dt);
		}
		adaptPlayerVelocity();

		// Handle collisions
		handleCollision();

		// Destroy all wrecks on the battlefield
		sceneGraph_.removeWrecks();

		// Spawn enemies
		spawnEnemies();

		// Regular update step, and adapt position of aircraft
		sceneGraph_.update(dt, getCommandQueue());
		adaptPlayerPosition();

		//Update sound
		updateSound();

		//Update score and multiplier texts
		updateScoreAndMultiplier();
		
		//Make enemies chase the player
		enemiesChasePlayer();

		//Play a zombie groan at regular intervals
		playZombieGroan();
	}

	//Update score and multiplier labels
	void World::updateScoreAndMultiplier()
	{
		scoreText_.setString("Score " + std::to_string(score_));
		multiplierText_.setString("X" + std::to_string(multiplier_));
	}

	void World::adaptPlayerVelocity()
	{
		sf::Vector2f velocity = player_->getVelocity();

		if (velocity.x != 0.f && velocity.y != 0.f)
			player_->setVelocity(velocity / std::sqrt(2.f));
	}

	void World::adaptPlayerPosition()
	{
		const float BORDER_DISTANCE = 40.f;
		sf::FloatRect viewBounds(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());

		sf::Vector2f position = player_->getPosition();
		position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE);

		position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE);

		player_->setPosition(position);
	}

	void World::adaptEnemies()
	{
		
	}

	void World::updateSound()
	{
		sounds_.setListenerPosition(player_->getWorldPosition());
		sounds_.removeStoppedSounds();
	}

		// Add spawn points for enemies outside the local view
	void World::addEnemies()
	{
		/*addEnemy(Aircraft::Type::Raptor, 0.f, 500.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 1000.f);
		addEnemy(Aircraft::Type::Raptor, +100.f, 1150.f);
		addEnemy(Aircraft::Type::Raptor, -100.f, 1150.f);
		addEnemy(Aircraft::Type::Avenger, 70.f, 1500.f);
		addEnemy(Aircraft::Type::Avenger, -70.f, 1500.f);
		addEnemy(Aircraft::Type::Avenger, -70.f, 1710.f);
		addEnemy(Aircraft::Type::Avenger, 70.f, 1700.f);
		addEnemy(Aircraft::Type::Avenger, 30.f, 1850.f);
		addEnemy(Aircraft::Type::Raptor, 300.f, 2200.f);
		addEnemy(Aircraft::Type::Raptor, -300.f, 2200.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 2200.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 2500.f);
		addEnemy(Aircraft::Type::Avenger, -300.f, 2700.f);
		addEnemy(Aircraft::Type::Avenger, -300.f, 2700.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 3000.f);
		addEnemy(Aircraft::Type::Raptor, 250.f, 3250.f);
		addEnemy(Aircraft::Type::Raptor, -250.f, 3250.f);
		addEnemy(Aircraft::Type::Avenger, 0.f, 3500.f);
		addEnemy(Aircraft::Type::Avenger, 0.f, 3700.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 3800.f);
		addEnemy(Aircraft::Type::Avenger, 0.f, 4000.f);
		addEnemy(Aircraft::Type::Avenger, -200.f, 4200.f);
		addEnemy(Aircraft::Type::Raptor, 200.f, 4200.f);
		addEnemy(Aircraft::Type::Raptor, 0.f, 4400.f);

		std::sort(enemySpawnPoints_.begin(), enemySpawnPoints_.end(), 
			[](Spawnpoint lhs, Spawnpoint rhs)
		{
			return lhs.y < rhs.y;
		});*/
	}

	void World::addEnemy(Spawnpoint spawn)
	{
		/*Spawnpoint spawnpoint(type, spawnPosition_.x + relX, spawnPosition_.y - relY);
		enemySpawnPoints_.push_back(spawnpoint);*/
	}

	//Play a random zombie groan noise for atmosphere every 15 secomds
	void World::playZombieGroan()
	{
		zombieGroanTimer_ += zombieGroanClock_.restart();

		if (zombieGroanTimer_ >= sf::seconds(15))
		{
			SoundEffectID sound;

			//Randomize groan noise
			switch (randomInt(3))
			{ 
				case 0:
					sound = SoundEffectID::ZombieGroan1;
					break;
				case 1:
					sound = SoundEffectID::ZombieGroan2;
					break;
				case 2:
					sound = SoundEffectID::ZombieGroan3;
					break;
				default:
					//Do Nothing
					break;
			}

			player_->playLocalSound(commandQueue_, sound);
			zombieGroanTimer_ -= sf::seconds(15);
		}
	}

	void World::spawnEnemies()
	{
		enemySpawnTimer_ += enemySpawnClock_.restart();

		while (enemySpawnTimer_ >= enemySpawnDelay_)
		{ 
			if (activeZombies_.size() < 10)
			{
				//TODO: Implement enemy randomizer here
				auto spawnpoint = enemySpawnPoints_[randomInt(3)];
				std::unique_ptr<Zombie> enemy(new Zombie(Zombie::ZombieType::Zombie, textures_));
				enemy->setPosition(spawnpoint.x, spawnpoint.y);
				activeZombies_.push_back(enemy.get());
				sceneLayers_[Ground]->attachChild(std::move(enemy));
			}

			enemySpawnTimer_ -= enemySpawnDelay_;
		}
	}

	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(worldView_.getCenter() - worldView_.getSize() / 2.f, worldView_.getSize());
	}

	sf::FloatRect World::getBattlefieldBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100.f;
		bounds.height += 100.f;
		return bounds;
	}

	void World::enemiesChasePlayer()
	{
		if (player_->getHitpoints() > 0)
		{ 
			for (auto e : activeZombies_)
			{
				//If zombie is alive, chase player
				if (e->getType() == Zombie::ZombieType::Zombie && e->getState() != Zombie::State::Dead)
				{
					auto d = distance(*player_, *e);

					sf::Vector2f velocityTemp = unitVector((player_->getWorldPosition() - e->getWorldPosition()) * 10000.f);

					if (std::abs(velocityTemp.x) > std::abs(velocityTemp.y))
					{
						velocityTemp.y = 0;
					}
					else
					{
						velocityTemp.x = 0;
					}

					e->setVelocity(velocityTemp * e->getMaxSpeed());
				}
				//If zombie is dead, set velocity to 0
				else if (e->getType() == Zombie::ZombieType::Zombie && e->getState() == Zombie::State::Dead)
					e->setVelocity(0.f, 0.f);
			}
		}
	}

	void World::guideMissiles()
	{
		//// build a list of active Enemies
		//Command enemyCollector;
		//enemyCollector.category = Category::EnemyAircraft;
		//enemyCollector.action = derivedAction<Player>([this](Player& enemy, sf::Time dt)
		//{
		//	if (!enemy.isDestroyed())
		//		activeEnemies_.push_back(&enemy);
		//});

		//Command missileGuider;
		//missileGuider.category = Category::Type::AlliedProjectile;
		//missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		//{
		//	//ignore bullets
		//	if (!missile.isGuided())
		//		return;

		//	float minDistance = std::numeric_limits<float>::max();
		//	Player* closestEnemy = nullptr;

		//	for (Player* e : activeEnemies_)
		//	{
		//		auto d = distance(missile, *e);
		//		if (d < minDistance)
		//		{
		//			minDistance = d;
		//			closestEnemy = e;
		//		}
		//	}

		//	if (closestEnemy)
		//		missile.guidedTowards(closestEnemy->getWorldPosition());
		//});

		//commandQueue_.push(enemyCollector);
		//commandQueue_.push(missileGuider);
		//activeEnemies_.clear();
	}

	bool matchesCategory(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		// Make sure pair entry has category type1 and second has type2
		if (type1 & category1 && type2 & category2)
		{
			return true;
		}
		else if (type1 & category2 && type2 & category1)
		{
			std::swap(colliders.first, colliders.second);
			return true;
		}
		else
		{
			return false;
		}
	}

	void World::handleCollision()
	{
		//build a list of colliding pairs of SceneNodes
		std::set<SceneNode::Pair> collisionPairs;
		sceneGraph_.checkSceneCollision(sceneGraph_, collisionPairs);

		for (SceneNode::Pair pair : collisionPairs)
		{
			//Player and Zombie
			if (matchesCategory(pair, Category::Type::Player, Category::Type::Zombie))
			{
				auto& player = static_cast<Player&>(*pair.first);
				auto& zombie = static_cast<Zombie&>(*pair.second);

				zombie.setVelocity(0.f, 0.f);
				zombie.setAttackInterval(zombie.getAttackInterval() + zombie.getAttackClock().restart());

				/*if (zombie.getAttackInterval() > sf::seconds(1.5))
					zombie.setAttackInterval(sf::seconds(1));*/

				if (zombie.getAttackInterval() >= zombie.getAttackDelay())
					player.damage(zombie.getDamage());

				zombie.setAttackInterval(zombie.getAttackInterval() - zombie.getAttackDelay());
			}
			//Player and Pickup
			else if (matchesCategory(pair, Category::Type::Player, Category::Type::Pickup))
			{
				auto& player = static_cast<Player&>(*pair.first);
				auto& pickup = static_cast<Pickup&>(*pair.second);

				pickup.apply(player);
				pickup.destroy();

				player_->playLocalSound(commandQueue_, SoundEffectID::CollectPickup);
			}
			//Zombie and Bullet
			else if (matchesCategory(pair, Category::Type::Zombie, Category::Type::AlliedProjectile))
			{
				auto& zombie = static_cast<Zombie&>(*pair.first);
				auto& projectile = static_cast<Projectile&>(*pair.second);

 				zombie.damage(projectile.getDamage());

				//If zombie is killed, update score
				if (zombie.getHitpoints() <= 0)
				{
					if (multiplier_ != 0)
						score_ += 200 * multiplier_;
					else
						score_ += 200;
				}

				projectile.destroy();
			}
			//Zombie and Zombie
			else if (matchesCategory(pair, Category::Type::Zombie, Category::Type::Zombie))
			{
				auto& zombie = static_cast<Zombie&>(*pair.first);
				auto& zombie2 = static_cast<Zombie&>(*pair.second);

				//Just move zombie a bit for now
				//TODO: Find a better way to handle this
				zombie.setPosition(zombie.getPosition().x + 10.f, zombie.getPosition().y + 10.f);

				/*if (zombie.getWorldPosition().x > zombie2.getWorldPosition().x)
				{
					zombie.setPosition(zombie2.getPosition().x - 13.f)
				}*/
			}
		}
	}

	void World::setupSpawnPoints()
	{
		Spawnpoint point1(-50.f, worldView_.getSize().y / 2.f);
		Spawnpoint point2(worldView_.getSize().x / 2.f, -50.f);
		Spawnpoint point3(1730.f, worldView_.getSize().y / 2.f);
		Spawnpoint point4(worldView_.getSize().x / 2.f, 1100.f);

		enemySpawnPoints_.push_back(point1);
		enemySpawnPoints_.push_back(point2);
		enemySpawnPoints_.push_back(point3);
		enemySpawnPoints_.push_back(point4);
	}

	void World::destroyEntitiesOutOfView()
	{
		Command command;
		command.category = Category::Type::Projectile/* | Category::Type::EnemyAircraft*/;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			if (!getBattlefieldBounds().intersects(e.getBoundingBox()))
				e.remove();
		});

		commandQueue_.push(command);
	}

	void World::draw()
	{
		target_.setView(worldView_);
		target_.draw(sceneGraph_);
		target_.draw(scoreText_);
		target_.draw(multiplierText_);
	}

	CommandQueue& World::getCommandQueue()
	{
		return commandQueue_;
	}

	bool World::hasAlivePlayer() const
	{
		return !player_->isDestroyed();
	}

	bool World::hasPlayerReachedEnd() const
	{
		return !worldBounds_.contains(player_->getPosition());
	}

	void World::loadTextures()
	{
		textures_.load(GEX::TextureID::Entities, "Media/Textures/Entities.png");
		//textures_.load(GEX::TextureID::Jungle, "Media/Textures/JungleBig.png");
		textures_.load(GEX::TextureID::Particle, "Media/Textures/Particle.png");
		textures_.load(GEX::TextureID::Explosion, "Media/Textures/Explosion.png");
		//textures_.load(GEX::TextureID::FinishLine, "Media/Textures/FinishLine.png");
		textures_.load(GEX::TextureID::LunarBackground, "Media/Textures/lunar_background.png");

		//Zombie and Skeleton
		textures_.load(GEX::TextureID::Zombie, "Media/Textures/zombie.png");
		textures_.load(GEX::TextureID::Skeleton, "Media/Textures/undeadking.png");

		//Animations
		//Zombies
		textures_.load(GEX::TextureID::ZombieWalkUp, "Media/Textures/zombie_walk_up.png");
		textures_.load(GEX::TextureID::ZombieWalkLeft, "Media/Textures/zombie_walk_left.png");
		textures_.load(GEX::TextureID::ZombieWalkDown, "Media/Textures/zombie_walk_down.png");
		textures_.load(GEX::TextureID::ZombieWalkRight, "Media/Textures/zombie_walk_right.png");
		textures_.load(GEX::TextureID::ZombieDeath, "Media/Textures/zombie_death.png");

		//Skeletons
		textures_.load(GEX::TextureID::SkeletonWalkUp, "Media/Textures/undeadking_walk_up.png");
		textures_.load(GEX::TextureID::SkeletonWalkLeft, "Media/Textures/undeadking_walk_left.png");
		textures_.load(GEX::TextureID::SkeletonWalkDown, "Media/Textures/undeadking_walk_down.png");
		textures_.load(GEX::TextureID::SkeletonWalkRight, "Media/Textures/undeadking_walk_right.png");

		//Player
		textures_.load(GEX::TextureID::PlayerWalkUp, "Media/Textures/player_walk_up.png");
		textures_.load(GEX::TextureID::PlayerWalkLeft, "Media/Textures/player_walk_left.png");
		textures_.load(GEX::TextureID::PlayerWalkDown, "Media/Textures/player_walk_down.png");
		textures_.load(GEX::TextureID::PlayerWalkRight, "Media/Textures/player_walk_right.png");

		textures_.load(GEX::TextureID::PlayerIdleUp, "Media/Textures/player_idle_up.png");
		textures_.load(GEX::TextureID::PlayerIdleLeft, "Media/Textures/player_idle_left.png");
		textures_.load(GEX::TextureID::PlayerIdleDown, "Media/Textures/player_idle_down.png");
		textures_.load(GEX::TextureID::PlayerIdleRight, "Media/Textures/player_idle_right.png");

		textures_.load(GEX::TextureID::PlayerDeath, "Media/Textures/player_death.png");
	}

	void World::buildScene()
	{
		// Initialize layers
		for (int i = 0; i < LayerCount; i++)
		{
			auto category = i == UpperAir ? Category::Type::SceneAirLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			sceneLayers_.push_back(layer.get());
			sceneGraph_.attachChild(std::move(layer));
		}

		//Sound
		std::unique_ptr<SoundNode> sNode(new SoundNode(sounds_));
		sceneGraph_.attachChild(std::move(sNode));

		// Particle System
		std::unique_ptr<ParticleNode> smoke(new ParticleNode(Particle::Type::Smoke, textures_));
		sceneLayers_[LowerAir]->attachChild(std::move(smoke));

		std::unique_ptr<ParticleNode> fire(new ParticleNode(Particle::Type::Propellant, textures_));
		sceneLayers_[LowerAir]->attachChild(std::move(fire));

		// draw background
		sf::Texture& texture = textures_.get(TextureID::LunarBackground);
		sf::IntRect textureRect(worldBounds_);
		texture.setRepeated(true);

		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(worldBounds_.left, worldBounds_.top);
		sceneLayers_[Background]->attachChild(std::move(backgroundSprite));

		// add player aircraft & game objects
			//player
		std::unique_ptr<Player> leader(new Player(Player::Type::Player, textures_));
		leader->setPosition(spawnPosition_);
		/*leader->setVelocity(50.f, scrollSpeed_);*/
		player_ = leader.get();
		sceneLayers_[UpperAir]->attachChild(std::move(leader));

		// add enemy planes
		/*addEnemies();*/
	}
}