/**
* @file
* @author
* Justin Lange 2018
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

#include "GameOverState.h"
#include "FontManager.h"
#include "Utility.h"
#include "GameState.h"

GameOverState::GameOverState(GEX::StateStack& stateStack, Context context)
	: State(stateStack, context)
	, gameOverText_()
	, elapsedTime_(sf::Time::Zero)
{
	sf::Font& font = GEX::FontManager::getInstance().get(GEX::FontID::Main);
	sf::Vector2f windowSize(context.window->getSize());

	gameOverText_.setFont(font);

	if (context.player->getCurrentMissionStatus() == GEX::MissionStatus::MissionFailure)
		gameOverText_.setString("You Died!");
	else
		gameOverText_.setString("Mission Accomplished");

	gameOverText_.setCharacterSize(70);
	GEX::centerOrigin(gameOverText_);
	gameOverText_.setPosition(windowSize.x * 0.5f, windowSize.y * 0.5f);
}

void GameOverState::draw()
{
	//sf::RenderWindow& window = *getContext().window;
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(gameOverText_);
}

bool GameOverState::update(sf::Time dt)
{
	elapsedTime_ += dt;

	if (elapsedTime_ > sf::seconds(3))
	{ 
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return false;
}

bool GameOverState::handleEvent(const sf::Event & event)
{
	return false;
}
