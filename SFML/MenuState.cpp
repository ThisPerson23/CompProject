/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* MenuState Class
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

#include "MenuState.h"
#include "Utility.h"
#include "FontManager.h"


MenuState::MenuState(GEX::StateStack& stateStack, Context context)
	: State(stateStack, context)
	, options_()
	, optionsIndex_(0)
{
	sf::Texture& texture = context.textures->get(GEX::TextureID::TitleScreen);

	backgroundSprite_.setTexture(texture);

	// set up menu

	// play option
	sf::Text playOption;
	playOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Spooky));
	playOption.setString("Play");
	GEX::centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	options_.push_back(playOption);

	// exit option
	sf::Text exitOption;
	exitOption.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Spooky));
	exitOption.setString("Exit");
	GEX::centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	options_.push_back(exitOption);

	updateOptionText();

	context.music_->play(GEX::MusicID::MenuTheme);
}

void MenuState::draw()
{
	//sf::RenderWindow& window = *getContext().window;
	auto& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(backgroundSprite_);

	for (const sf::Text& text : options_)
		window.draw(text);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event & event)
{
		// If it is not a key press event, don't care about it
	if (event.type != sf::Event::KeyPressed)
		return true;

		
	if (event.key.code == sf::Keyboard::Return)
	{
		if (optionsIndex_ == Play)
		{
			requestStackPop();
			requestStackPush(GEX::StateID::Game);
		}
		else if (optionsIndex_ == Exit)
		{
			requestStackPop();
		}
	}
	else if (event.key.code == sf::Keyboard::Up)
	{
		if (optionsIndex_ > 0)
			optionsIndex_--;
		else
			optionsIndex_ = options_.size() - 1;

		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		if (optionsIndex_ < options_.size() - 1)
			optionsIndex_++;
		else
			optionsIndex_ = 0;

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (options_.empty())
		return;

	for (sf::Text& text : options_)
	{
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
	}

	options_[optionsIndex_].setFillColor(sf::Color::Magenta);
}
