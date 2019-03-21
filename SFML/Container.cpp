/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Container Class
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

#include "Container.h"

#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

namespace GUI
{ 
	Container::Container()
		: children_()
		, selectedChild_()
	{}

	void Container::pack(Component::Ptr component)
	{
		children_.push_back(component);

		if (!hasSelection() && component->isSelectable())
			select(children_.size() - 1);
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event & event)
	{
		//If a child is selected, give it events
		if (hasSelection() && children_[selectedChild_]->isActive())
		{
			children_[selectedChild_]->handleEvent(event);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
					children_[selectedChild_]->activate();
			}
		}
	}

	void Container::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (auto& child : children_)
			target.draw(*child, states);
	}

	bool Container::hasSelection() const
	{
		return selectedChild_ >= 0;
	}
	void Container::select(std::size_t index)
	{
		if (children_[index]->isSelectable())
		{
			if (hasSelection())
				children_[selectedChild_]->deselect();

			children_[index]->select();
			selectedChild_ = index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		//Search next component which is selectable, wrap around if necessary
		int next = selectedChild_;

		do
		{
			next = (next + 1) % children_.size();
		} while (!children_[next]->isSelectable());

		//Select that component
		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		//Search previous component that is selectable, wrap around if necessary
		int prev = selectedChild_;

		do
		{
			prev = (prev + children_.size() - 1) % children_.size();
		} while (!children_[prev]->isSelectable());

		//Select that component
		select(prev);
	}
}