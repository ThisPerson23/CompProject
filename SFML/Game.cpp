/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Game Class
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

#include "Game.h"
#include <string>

namespace
{
	const sf::IntRect NO_ROLL(0, 0, 48, 64);
	const sf::IntRect ROLL_LEFT(48, 0, 48, 64);
	const sf::IntRect ROLL_RIGHT(96, 0, 48, 64);
}

namespace GEX
{ 
	Game::~Game()
	{
	}

	Game::Game()
		: window_(sf::VideoMode(1680, 1050), "Boxhead")
		, world_(window_, sounds_)
		, statisticsText_()
		, statisticsUpdateTime_(sf::Time::Zero)
		, statisticsNumFrames_(0)
	{
		

		if (!font_.loadFromFile("Media/Sansation.ttf"))
		{ 
			//error handling
		}

		statisticsText_.setFont(font_);
		statisticsText_.setPosition(5.f, 5.f);
		statisticsText_.setCharacterSize(12.f);
		statisticsText_.setString("Frames Per Second = \nTime / Update = ");
	}

		//run game
	void Game::run()
	{
		const sf::Time SPF = sf::seconds(1.0f / 60.0f);		//seconds per frame for 60 fps

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;


		while (window_.isOpen())
		{
			processInput();
			timeSinceLastUpdate = clock.restart();

			while (timeSinceLastUpdate > SPF)
			{
				update(SPF, world_.getCommandQueue());
				updateStatistics(SPF);
				timeSinceLastUpdate -= SPF;
			}

			updateStatistics(timeSinceLastUpdate);
			update(timeSinceLastUpdate, world_.getCommandQueue());
			render();
		}
	}

		//handle button presses, releases, and window closing
	void Game::processInput()
	{
		CommandQueue& commands = world_.getCommandQueue();

		sf::Event event;

		while (window_.pollEvent(event))
		{
			player_.handleEvent(event, commands);
			
			if (sf::Event::Closed)
				window_.close();
		}

		player_.handleRealtimeInput(commands);
	}

	void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
	{

	}

	void Game::updateStatistics(sf::Time deltaTime)
	{
		statisticsUpdateTime_ += deltaTime;
		statisticsNumFrames_ += 1;

		if (statisticsUpdateTime_ > sf::seconds(1))
		{
			statisticsText_.setString("Frames Per Second = " + std::to_string(statisticsNumFrames_) + "\n" +
									  "Time / Update = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_));

			statisticsUpdateTime_ -= sf::seconds(1);
			statisticsNumFrames_ = 0;
		}
	}

	void Game::update(sf::Time dt, CommandQueue& commands)
	{
		world_.update(dt, commands);
	}

	void Game::render()
	{
		window_.clear();
		world_.draw();

		window_.setView(window_.getDefaultView());

		window_.draw(statisticsText_);
		window_.display();
	}

}