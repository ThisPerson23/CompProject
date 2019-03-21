/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* SoundPlayer Class
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

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Vector2.hpp>

#include "ResourceIdentifiers.h"

#include <map>
#include <memory>
#include <list>
#include <string>

namespace GEX
{ 
	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer() = default;
		SoundPlayer(const SoundPlayer&) = delete;
		SoundPlayer& operator=(const SoundPlayer&) = delete;

		void play(SoundEffectID effect);
		void play(SoundEffectID effect, sf::Vector2f position);
		void removeStoppedSounds();
		void setListenerPosition(sf::Vector2f position);
		sf::Vector2f getListenerPosition() const;

	private:
		void loadBuffer(SoundEffectID id, const std::string path);

	private:
		std::map<SoundEffectID, std::unique_ptr<sf::SoundBuffer>> soundBuffers_;
		std::list<sf::Sound> sounds_;
		float volume_;
	};
}