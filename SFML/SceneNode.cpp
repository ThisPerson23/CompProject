/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* SceneNode Class
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

#include <algorithm>
#include <cassert>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "SceneNode.h"
#include "Category.h"
#include "Command.h"
#include "Utility.h"

namespace GEX
{ 
	SceneNode::SceneNode(Category::Type category)
		: children_()
		, parent_(nullptr)
		, category_(category)
	{}

	void SceneNode::attachChild(Ptr child)
	{
		child->parent_ = this;
		children_.push_back(std::move(child));
	}

	SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
	{
		auto found = std::find_if
								(
								 children_.begin(), 
								 children_.end(),
								 [&](Ptr& p) { return &node == p.get(); }
								);

		assert(found != children_.end());

		Ptr result = std::move(*found);
		children_.erase(found);

		return result;
	}

	sf::Vector2f SceneNode::getWorldPosition() const
	{
		return getWorldTransform() * sf::Vector2f();
	}

	sf::Transform SceneNode::getWorldTransform() const
	{
		sf::Transform transform = sf::Transform::Identity;

		for (const SceneNode* node = this; node != nullptr; node = node->parent_)
			transform = node->getTransform() * transform;

		return transform;
	}

	sf::FloatRect SceneNode::getBoundingBox() const
	{
		return sf::FloatRect();
	}

	void SceneNode::drawBoundingBox(sf::RenderTarget & target, sf::RenderStates states) const
	{
		/*sf::FloatRect rect = getBoundingBox();

		sf::RectangleShape box;
		box.setPosition(sf::Vector2f(rect.left, rect.top));
		box.setSize(sf::Vector2f(rect.width, rect.height));
		box.setFillColor(sf::Color::Transparent);
		box.setOutlineColor(sf::Color::Cyan);
		box.setOutlineThickness(1.f);

		target.draw(box);*/
	}

	bool SceneNode::isDestroyed() const
	{
		return false;
	}

	bool SceneNode::isMarkedForRemoval() const
	{
		return isDestroyed();
	}

	void SceneNode::removeWrecks()
	{
		auto wreckFieldBegin = std::remove_if(children_.begin(), children_.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
		children_.erase(wreckFieldBegin, children_.end());

		std::for_each(children_.begin(), children_.end(), std::mem_fn(&SceneNode::removeWrecks));
	}

	void SceneNode::checkSceneCollision(SceneNode & node, std::set<Pair>& collisionPair)
	{
		checkNodeCollision(node, collisionPair);

		for (Ptr& c : node.children_)
			checkSceneCollision(*c, collisionPair);
	}

	void SceneNode::checkNodeCollision(SceneNode & node, std::set<Pair>& collisionPair)
	{
		if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
			collisionPair.insert(std::minmax(this, &node));

		for (Ptr& c : children_)
			c->checkNodeCollision(node, collisionPair);
	}

	void SceneNode::update(sf::Time dt, CommandQueue& commands)
	{
		updateCurrent(dt, commands);
		updateChildren(dt, commands);
	}

	void SceneNode::onCommand(const Command & command, sf::Time dt)
	{
		if (command.category & getCategory())
			command.action(*this, dt);

			// Command children
		for (Ptr& child : children_)
			child->onCommand(command, dt);
	}

	unsigned int SceneNode::getCategory() const
	{
		return category_;
	}

	void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		//default to do nothing.
	}

	void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
	{
		for (Ptr& child : children_)
		{
			child->update(dt, commands);
		}
	}

	void SceneNode::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		drawCurrent(target, states);
		drawChildren(target, states);

		drawBoundingBox(target, states);
	}

	void SceneNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		//default to do nothing.
	}

	void SceneNode::drawChildren(sf::RenderTarget & target, sf::RenderStates states) const
	{
		for (const Ptr& child : children_)
		{
			child->draw(target, states);
		}
	}
	
	float distance(const SceneNode & lhs, const SceneNode & rhs)
	{
		return length(lhs.getWorldPosition() - rhs.getWorldPosition());
	}

	bool collision(const SceneNode & lhs, const SceneNode & rhs)
	{
		return lhs.getBoundingBox().intersects(rhs.getBoundingBox());
	}

}