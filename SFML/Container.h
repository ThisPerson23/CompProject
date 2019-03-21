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

#pragma once

#include "Component.h"

#include <vector>
#include <memory>

namespace GUI
{ 
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
						Container();
		void			pack(Component::Ptr component);

		virtual bool	isSelectable() const;
		virtual void	handleEvent(const sf::Event& event);

	private:
		virtual void	draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool			hasSelection() const;
		void			select(std::size_t index);
		void			selectNext();
		void			selectPrevious();

	private:
		std::vector<Component::Ptr>		children_;
		int								selectedChild_;
	};
}
