/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* World Class
* The game's world
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

#include <SFML\Graphics\View.hpp>
#include <SFML\Graphics\Texture.hpp>

#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "Player.h"
#include "Category.h"
#include "CommandQueue.h"
#include "SoundPlayer.h"
#include "Zombie.h"
#include "Skeleton.h"

#include <vector>

namespace sf
{
	class RenderTarget;
}

namespace GEX
{ 
	struct Spawnpoint
	{
		Spawnpoint(/*Aircraft::Type _type, */float _x, float _y)
			/*: type(_type)*/
			: x(_x)
			, y(_y)
		{}

		/*Aircraft::Type type;*/
		float			x;
		float			y;
	};

	class World
	{
	public:
		explicit					World(sf::RenderWindow& window, SoundPlayer& sounds);

		void						update(sf::Time dt, CommandQueue& commands);
		void						draw();

		CommandQueue&				getCommandQueue();

		bool						hasAlivePlayer() const;

	private:
		void						loadTextures();
		void						buildScene();
		void						adaptPlayerVelocity();
		void						adaptPlayerPosition();

		void						updateScoreAndMultiplier();

		void						cleanEnemyVector();
		void						spawnEnemies();

		sf::FloatRect				getViewBounds() const;
		sf::FloatRect				getBattlefieldBounds() const;

		void						enemiesChasePlayer();

		void						handleCollision();

		void						playZombieGroan();

		void						setupSpawnPoints();	

		void						destroyEntitiesOutOfView();

		void						updateSound();

	private:
		enum Layer 
		{
			Background = 0,
			LowerGround,
			Ground,
			LayerCount
		};

	private:
		sf::RenderWindow&			target_;
		sf::View					worldView_;
		TextureManager				textures_;
		SoundPlayer&				sounds_;

		SceneNode					sceneGraph_;
		std::vector<SceneNode*>		sceneLayers_;

		CommandQueue				commandQueue_;
		sf::FloatRect				worldBounds_;
		sf::Vector2f				spawnPosition_;
		float						scrollSpeed_;
		Player*						player_;

		std::vector<Spawnpoint>		enemySpawnPoints_;

		std::vector<Zombie*>		activeZombies_;

		sf::Text					scoreText_;
		sf::Text					multiplierText_;
		int							multiplier_;
		int							score_;

		sf::Time					enemySpawnDelay_;
		sf::Time					enemySpawnTimer_;
		sf::Clock					enemySpawnClock_;

		sf::Time					zombieGroanTimer_;
		sf::Clock					zombieGroanClock_;
	};
}
