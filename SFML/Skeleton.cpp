/**
* @file
* @author
* Justin Lange 2019
* @version 1.0
*
*
* @section DESCRIPTION
* Skeleton Class
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

#include "Skeleton.h"
#include "DataTables.h"

namespace GEX
{ 
	namespace
	{
		const std::map<Skeleton::SkeletonType, SkeletonData> TABLE = initializeSkeletonData();
	}

	Skeleton::Skeleton(Skeleton::SkeletonType type, const TextureManager& textures)
		: Entity(TABLE.at(type).hitpoints)
	{
	}
	void Skeleton::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
	}
	unsigned int Skeleton::getCategory() const
	{
		return 0;
	}
	bool Skeleton::isMarkedForRemoval() const
	{
		return false;
	}
	void Skeleton::remove()
	{
	}
	void Skeleton::playLocalSound(CommandQueue & commands, SoundEffectID effect)
	{
	}
	void Skeleton::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
	}
	float Skeleton::getMaxSpeed() const
	{
		return 0.0f;
	}
	void Skeleton::updateStates(sf::Time dt)
	{
	}
	void Skeleton::setState(Skeleton::State state)
	{
	}
	Skeleton::State Skeleton::getState() const
	{
		return Skeleton::State();
	}
	void Skeleton::createPickup(SceneNode & node, const TextureManager & textures) const
	{
	}
	void Skeleton::checkPickupDrop(CommandQueue & commands)
	{
	}
	void Skeleton::setupAnimations()
	{
	}
}