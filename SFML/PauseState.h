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

#pragma once

#include "State.h"

class PauseState : public GEX::State
{
	public:
		PauseState(GEX::StateStack& stateStack, Context context);
		~PauseState();
	
		void					draw() override;
		bool					update(sf::Time dt) override;
		bool					handleEvent(const sf::Event& event) override;

	private:
		sf::Sprite				backgroundSprite_;
		sf::Text				pausedText_;
		sf::Text				instructionText_;

};

