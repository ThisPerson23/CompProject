/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* GameState Class
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

#include "GameState.h"
#include "CommandQueue.h"

GameState::GameState(GEX::StateStack& stateStack, Context context)
	: State(stateStack, context)
	, world_(*context.window, *context.sound_)
	, player_(*context.player)
{
	context.music_->play(GEX::MusicID::GameTheme);
}

void GameState::draw()
{
	world_.draw();
}

bool GameState::update(sf::Time dt)
{
		//update the world and handle player inputs
	world_.update(dt, world_.getCommandQueue());

	if (!world_.hasAlivePlayer())
	{
		player_.setCurrentMissionStatus(GEX::MissionStatus::MissionFailure);
		requestStackPush(GEX::StateID::GameOver);
	}
	else if (world_.hasPlayerReachedEnd())
	{
		player_.setCurrentMissionStatus(GEX::MissionStatus::MissionSuccess);
		requestStackPush(GEX::StateID::GameOver);
	}

	//GEX::CommandQueue& commands = world_.getCommandQueue();
	auto& commands = world_.getCommandQueue();
	player_.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	auto& commands = world_.getCommandQueue();
	player_.handleEvent(event, commands);

		//'Escape' key brings up pause screen, 'G' key brings up GEX screen, 'Q' key returns player to main menu instantly
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(GEX::StateID::Pause);
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
		requestStackPush(GEX::StateID::GEXScreen);
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
	{
		requestStackClear();
		requestStackPush(GEX::StateID::Menu);
	}

	return true;
}