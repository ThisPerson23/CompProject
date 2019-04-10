/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* MusicPlayer Class
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

#include "MusicPlayer.h"


namespace GEX
{
	MusicPlayer::MusicPlayer()
		: music_()
		, filenames_()
		, volume_(10.f)
	{
		filenames_[MusicID::GameTheme] = "Media/Music/ZombieGameTheme.ogg";
		filenames_[MusicID::MenuTheme] = "Media/Music/ZombieMenuTheme.wav";
	}

	void MusicPlayer::play(MusicID theme)
	{
		if (!music_.openFromFile(filenames_.at(theme)))
		{
			throw std::runtime_error("Music could not open file");
		}

		music_.setVolume(volume_);
		music_.setLoop(true);
		music_.play();
	}

	void MusicPlayer::stop()
	{
		music_.stop();
	}

	void MusicPlayer::setPaused(bool paused)
	{
		if (paused)
		{
			music_.pause();
		}
		else
		{
			music_.play();
		}
	}

	void MusicPlayer::setVolume(float volume)
	{
		volume_ = volume;
		music_.setVolume(volume_);
	}
}