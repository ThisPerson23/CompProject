/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* GEXState Class
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

#include "GEXState.h"
#include "Utility.h"
#include "CommandQueue.h"
#include "FontManager.h"


GEXState::GEXState(GEX::StateStack& stateStack, Context context)
	: State(stateStack, context)
	, backgroundImage_()
	, pauseText_()
	, stateText_()
	, instructionsTextReturnToMenu_()
	, instructionsTextReturnToGame_()
{
		//get the texture from the manager, get the font and view size from the context
	sf::Texture& texture = context.textures->get(GEX::TextureID::GEXStateFace);
	sf::Vector2f viewSize = context.window->getView().getSize();

		//center the image's origin and approximately center it. NEEDS WORK!!!!
	GEX::centerOrigin(backgroundImage_);
	backgroundImage_.setPosition(0.35f * viewSize.x, 0.3f * viewSize.y);

		//set the texture for the background image and center it
	backgroundImage_.setTexture(texture);
	backgroundImage_.setColor(sf::Color(255, 255, 255, 100));
		
		//set up 'Game Paused' text
	pauseText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	pauseText_.setString("Game Paused");
	pauseText_.setCharacterSize(80);
	GEX::centerOrigin(pauseText_);
	pauseText_.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

		//set up 'GEX STATE' text
	stateText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	stateText_.setString("GEX STATE");
	stateText_.setCharacterSize(60);
	GEX::centerOrigin(stateText_);
	stateText_.setPosition(0.5f * viewSize.x, 0.5f * viewSize.y);

		//set up 'Press G to return to game' and 'Press Escape to return to main menu' text
	instructionsTextReturnToGame_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	instructionsTextReturnToGame_.setString("Press 'G' to Return to the Game");
	GEX::centerOrigin(instructionsTextReturnToGame_);
	instructionsTextReturnToGame_.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);

	instructionsTextReturnToMenu_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	instructionsTextReturnToMenu_.setString("Press 'Backspace' to Return to the Main Menu");
	GEX::centerOrigin(instructionsTextReturnToMenu_);
	instructionsTextReturnToMenu_.setPosition(0.5f * viewSize.x, 0.7f * viewSize.y);
}

void GEXState::draw()
{
	//sf::RenderWindow& window = *getContext().window;
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());

		//give the screen a red transparent background when GEX State is active
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(128, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

		//draw all objects
		//Layer 1
	window.draw(backgroundShape);
		//Layer 2
	window.draw(backgroundImage_);
		//Layer 3
	window.draw(pauseText_);
	window.draw(stateText_);
	window.draw(instructionsTextReturnToGame_);
	window.draw(instructionsTextReturnToMenu_);
}

bool GEXState::update(sf::Time dt)
{
	return false;
}

bool GEXState::handleEvent(const sf::Event & event)
{
		//check if event is key press event
	if (event.type != sf::Event::KeyPressed)
		return false;

		//if 'G', return the player to the game
	if (event.key.code == sf::Keyboard::G)
		requestStackPop();

		//if 'backspace', return the player to the main menu
	if (event.key.code == sf::Keyboard::BackSpace)
	{
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return false;
}
