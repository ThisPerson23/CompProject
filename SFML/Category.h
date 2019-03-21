/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Category Helper Class
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

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		PlayerAircraft		= 1 << 1,
		AlliedAircraft		= 1 << 2,
		EnemyAircraft		= 1 << 3,
		EnemyProjectile		= 1 << 4,
		AlliedProjectile	= 1 << 5,
		SceneAirLayer		= 1 << 6,
		Pickup				= 1 << 7,
		ParticleSystem		= 1 << 8,
		SoundEffect			= 1 << 9,
		Zombie				= 1 << 10,

		Aircraft	 = PlayerAircraft | AlliedAircraft | EnemyAircraft,
		Projectile	 = EnemyProjectile | AlliedProjectile
	};
}