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

#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "TextureManager.h"
#include "StateIdentifiers.h"
#include "CommandQueue.h"
#include "MusicPlayer.h"

#include <memory>

namespace GEX
{
	// Forward declarations
	class StateStack;
	class PlayerControl;
	class SoundPlayer;


	class State
	{
	public:
		typedef std::unique_ptr<State> Ptr;
		
		struct Context
		{
			Context
			(
				sf::RenderWindow&	 window,
				TextureManager& 	 textures,
				PlayerControl&		 player,
				MusicPlayer&		 music,
				SoundPlayer&         sound
			);

			sf::RenderWindow*	 window;
			TextureManager*		 textures;
			PlayerControl*		 player;
			MusicPlayer*		 music_;
			SoundPlayer*		 sound_;
		};

	public:
		State(StateStack& stack, Context context);
		virtual			~State();

		virtual void	draw() = 0;
		virtual bool	update(sf::Time dt) = 0;
		virtual bool	handleEvent(const sf::Event& event) = 0;

	protected:
		void			requestStackPush(StateID stateID);
		void			requestStackPop();
		void			requestStackClear();

		Context			getContext() const;

	private:
		StateStack*		stack_;
		Context			context_;
	};
}
