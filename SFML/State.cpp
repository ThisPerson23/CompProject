/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* State Class
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

#include "State.h"
#include "StateStack.h"


namespace GEX
{ 
	State::Context::Context(
		sf::RenderWindow & window,
		TextureManager & textures,
		PlayerControl & player,
		MusicPlayer & music,
		SoundPlayer & sound)
		: window(&window)
		, textures(&textures)
		, player(&player)
		, music_(&music)
		, sound_(&sound)
	{}

	State::State(StateStack & stack, Context context)
		: stack_(&stack)
		, context_(context)
	{}

	State::~State()
	{}

	void State::requestStackPush(StateID stateID)
	{
		stack_->pushState(stateID);
	}

	void State::requestStackPop()
	{
		stack_->popState();
	}

	void State::requestStackClear()
	{
		stack_->clearStates();
	}

	State::Context State::getContext() const
	{
		return context_;
	}	
}