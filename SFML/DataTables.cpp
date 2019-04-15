/**
* @file
* @author
* Justin Lange 2019
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
		data[Zombie::ZombieType::Zombie].speed = 75.f;
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
		data[Player::Type::Player].speed = 150.f;
		data[Player::Type::Player].texture = TextureID::Entities;
		data[Player::Type::Player].textureRect = sf::IntRect(0, 0, 48, 64);
		data[Player::Type::Player].fireInterval = sf::seconds(0.5f);

		data[Player::Type::Player].textureIdleUp = TextureID::PlayerIdleUp;
		data[Player::Type::Player].textureIdleLeft = TextureID::PlayerIdleLeft;
		data[Player::Type::Player].textureIdleDown = TextureID::PlayerIdleDown;
		data[Player::Type::Player].textureIdleRight = TextureID::PlayerIdleRight;

		return data;
	}

	std::map<Pickup::Type, PickupData> GEX::initializePickupData()
	{
		std::map <Pickup::Type, PickupData> data;

		data[Pickup::Type::AmmoRefill].texture = TextureID::Entities;
		data[Pickup::Type::AmmoRefill].textureRect = sf::IntRect(120, 64, 40, 40);
		data[Pickup::Type::AmmoRefill].action = [](Player& a) {a.collectAmmo(15); };

		return data;
	}

	std::map<Projectile::Type, ProjectileData> GEX::initializeProjectileData()
	{
		std::map <Projectile::Type, ProjectileData> data;

		data[Projectile::Type::AlliedBullet].damage = 50;
		data[Projectile::Type::AlliedBullet].speed = 1500.f;
		data[Projectile::Type::AlliedBullet].texture = TextureID::Entities;
		data[Projectile::Type::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

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
