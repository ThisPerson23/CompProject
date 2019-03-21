/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Application class
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

#include "Application.h"
#include "GameState.h"
#include "PauseState.h"
#include "MenuState.h"
#include "TitleState.h"
#include "GEXState.h"
#include "GameOverState.h"
#include "FontManager.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.0f);		//seconds per frame for 60 fps


Application::Application()
	: window_(sf::VideoMode(1280, 960), "Killer Planes", sf::Style::Close)
	, player_()
	, textures_()
	, music_()
	, sound_()
	, stateStack_(GEX::State::Context(window_, textures_, player_, music_, sound_))
	, statisticsText_()
	, statisticsUpdateTime_()
	, statisticsNumFrames_(0)
{
	window_.setKeyRepeatEnabled(false);

	GEX::FontManager::getInstance().load(GEX::FontID::Main, "Media/Sansation.ttf");

	textures_.load(GEX::TextureID::TitleScreen, "Media/Menus/TitleScreenBig.png");
	textures_.load(GEX::TextureID::GEXStateFace, "Media/face.png");

	statisticsText_.setFont(GEX::FontManager::getInstance().get(GEX::FontID::Main));
	statisticsText_.setPosition(15.0f, 15.0f);
	statisticsText_.setCharacterSize(15);
	statisticsText_.setString("Frames Per Second = \nTime / Update = ");

	registerStates();
	stateStack_.pushState(GEX::StateID::Menu);

}
void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window_.isOpen())
	{
		timeSinceLastUpdate += clock.restart();

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (stateStack_.isEmpty())
				window_.close();
		}

		updateStatistics(timeSinceLastUpdate);

		render();
	}
}

void Application::processInput()
{
	sf::Event event;

	while (window_.pollEvent(event))
	{
		stateStack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window_.close();
	}
}

void Application::update(sf::Time dt)
{
	stateStack_.update(dt);
}

void Application::render()
{
	window_.clear();
	stateStack_.draw();

	window_.setView(window_.getDefaultView());
	window_.draw(statisticsText_);
	window_.display();
}

void Application::updateStatistics(sf::Time dt)
{
	statisticsUpdateTime_ += dt;
	statisticsNumFrames_ += 1;

	if (statisticsUpdateTime_ > sf::seconds(1))
	{
		statisticsText_.setString("Frames Per Second = " + std::to_string(statisticsNumFrames_) + "\n" +
			"Time / Update = " + std::to_string(statisticsUpdateTime_.asMicroseconds() / statisticsNumFrames_));

		statisticsUpdateTime_ -= sf::seconds(1);
		statisticsNumFrames_ = 0;
	}
}

void Application::registerStates()
{
	stateStack_.registerState<TitleState>(GEX::StateID::Title);
	stateStack_.registerState<MenuState>(GEX::StateID::Menu);
	stateStack_.registerState<GameState>(GEX::StateID::Game);
	stateStack_.registerState<PauseState>(GEX::StateID::Pause);
	stateStack_.registerState<GEXState>(GEX::StateID::GEXScreen);
	stateStack_.registerState<GameOverState>(GEX::StateID::GameOver);
}