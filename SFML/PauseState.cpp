/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* PauseState Class
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

#include "PauseState.h"
#include "Utility.h"
#include "FontManager.h"


PauseState::PauseState(GEX::StateStack& stateStack, Context context)
	: State(stateStack, context)
	, backgroundSprite_()
	, pausedText_()
	, instructionText_()
{
	sf::Vector2f viewSize = context.window->getView().getSize();

	pausedText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	pausedText_.setString("Game Paused");
	pausedText_.setCharacterSize(80);
	GEX::centerOrigin(pausedText_);
	pausedText_.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	instructionText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	instructionText_.setString("(Press Backspace to Return to the Main Menu)");
	GEX::centerOrigin(instructionText_);
	instructionText_.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);

	context.music_->setPaused(true);
}

void PauseState::draw()
{
	//sf::RenderWindow& window = *getContext().window;
	auto& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());
	
	window.draw(backgroundShape);
	window.draw(pausedText_);
	window.draw(instructionText_);	
}

bool PauseState::update(sf::Time dt)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event & event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape)
		requestStackPop();

	if (event.key.code == sf::Keyboard::BackSpace)
	{ 
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return false;
}