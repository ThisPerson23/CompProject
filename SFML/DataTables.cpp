/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* DataTables Class
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

#include "DataTables.h"

namespace GEX
{ 
	std::map<Zombie::ZombieType, ZombieData> GEX::initializeZombieData()
	{
		std::map<Zombie::ZombieType, ZombieData> data;

		data[Zombie::ZombieType::Zombie].hitpoints = 100;
		data[Zombie::ZombieType::Zombie].speed = 50.f;
		data[Zombie::ZombieType::Zombie].texture = TextureID::Zombie;
		data[Zombie::ZombieType::Zombie].attackInterval = sf::seconds(1);
		data[Zombie::ZombieType::Zombie].damage = 1;
		
		return data;
	}

	std::map<Skeleton::SkeletonType, SkeletonData> GEX::initializeSkeletonData()
	{
		std::map<Skeleton::SkeletonType, SkeletonData> data;

		data[Skeleton::SkeletonType::Skeleton].hitpoints = 400;
		data[Skeleton::SkeletonType::Skeleton].speed = 100.f;
		data[Skeleton::SkeletonType::Skeleton].texture = TextureID::Skeleton;
		data[Skeleton::SkeletonType::Skeleton].attackInterval = sf::seconds(1);

		return data;
	}

	std::map<Player::Type, PlayerData> GEX::initializePlayerData()
	{
		std::map <Player::Type, PlayerData> data;

		data[Player::Type::Player].hitpoints = 100;
		data[Player::Type::Player].speed = 200.f;
		data[Player::Type::Player].texture = TextureID::Entities;
		data[Player::Type::Player].textureRect = sf::IntRect(0, 0, 48, 64);
		data[Player::Type::Player].fireInterval = sf::seconds(1);
		data[Player::Type::Player].hasRollAnimation = true;

		data[Player::Type::Player].textureIdleUp = TextureID::PlayerIdleUp;
		data[Player::Type::Player].textureIdleLeft = TextureID::PlayerIdleLeft;
		data[Player::Type::Player].textureIdleDown = TextureID::PlayerIdleDown;
		data[Player::Type::Player].textureIdleRight = TextureID::PlayerIdleRight;

		/*data[Player::Type::Raptor].hitpoints = 20;
		data[Player::Type::Raptor].speed = 80.f;
		data[Player::Type::Raptor].texture = TextureID::Entities;
		data[Player::Type::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
		data[Player::Type::Raptor].fireInterval = sf::Time::Zero;
		data[Player::Type::Raptor].hasRollAnimation = false;

		data[Player::Type::Raptor].directions.emplace_back(Direction(45.f, 80.f));
		data[Player::Type::Raptor].directions.emplace_back(Direction(-45.f, 160.f));
		data[Player::Type::Raptor].directions.emplace_back(Direction(45.f, 80.f));

		data[Player::Type::Avenger].hitpoints = 40;
		data[Player::Type::Avenger].speed = 50.f;
		data[Player::Type::Avenger].texture = TextureID::Entities;
		data[Player::Type::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
		data[Player::Type::Avenger].fireInterval = sf::seconds(2);
		data[Player::Type::Avenger].hasRollAnimation = false;

		data[Player::Type::Avenger].directions.emplace_back(Direction(45.f, 50.f));
		data[Player::Type::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[Player::Type::Avenger].directions.emplace_back(Direction(-45.f, 100.f));
		data[Player::Type::Avenger].directions.emplace_back(Direction(0.f, 50.f));
		data[Player::Type::Avenger].directions.emplace_back(Direction(45.f, 50.f));*/

		return data;
	}

	std::map<Pickup::Type, PickupData> GEX::initializePickupData()
	{
		std::map <Pickup::Type, PickupData> data;

		data[Pickup::Type::HealthRefill].texture = TextureID::Entities;
		data[Pickup::Type::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
		data[Pickup::Type::HealthRefill].action = [](Player& a) {a.repair(25); };

		data[Pickup::Type::MissileRefill].texture = TextureID::Entities;
		data[Pickup::Type::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
		data[Pickup::Type::MissileRefill].action = [](Player& a) {a.collectMissiles(3); };

		data[Pickup::Type::FireSpread].texture = TextureID::Entities;
		data[Pickup::Type::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
		data[Pickup::Type::FireSpread].action = [](Player& a) {a.increaseFireSpread(); };

		data[Pickup::Type::FireRate].texture = TextureID::Entities;
		data[Pickup::Type::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
		data[Pickup::Type::FireRate].action = [](Player& a) {a.increaseFireRate(); };

		return data;
	}

	std::map<Projectile::Type, ProjectileData> GEX::initializeProjectileData()
	{
		std::map <Projectile::Type, ProjectileData> data;

		data[Projectile::Type::AlliedBullet].damage = 50;
		data[Projectile::Type::AlliedBullet].speed = 1500.f;
		data[Projectile::Type::AlliedBullet].texture = TextureID::Entities;
		data[Projectile::Type::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

		data[Projectile::Type::EnemyBullet].damage = 10;
		data[Projectile::Type::EnemyBullet].speed = 300.f;
		data[Projectile::Type::EnemyBullet].texture = TextureID::Entities;
		data[Projectile::Type::EnemyBullet].textureRect = sf::IntRect(175, 64, 3, 14);

		data[Projectile::Type::Missile].damage = 200;
		data[Projectile::Type::Missile].speed = 200.f;
		data[Projectile::Type::Missile].texture = TextureID::Entities;
		data[Projectile::Type::Missile].textureRect = sf::IntRect(160, 64, 15, 24);

		return data;
	}

	std::map<Particle::Type, ParticleData> GEX::initializeParticleData()
	{
		std::map <Particle::Type, ParticleData> data;

		data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
		data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

		data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
		data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

		return data;
	}
}
