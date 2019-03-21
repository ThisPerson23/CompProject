/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* Component Class
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

#include "Component.h"


namespace GUI
{ 
	Component::Component()
		: isSelected_(false)
		, isActive_(false)
	{}

	Component::~Component()
	{
	}

	bool GUI::Component::isSelected() const
	{
		return isSelected_;
	}

	void GUI::Component::select()
	{
		isSelected_ = true;
	}

	void GUI::Component::deselect()
	{
		isSelected_ = false;
	}

	bool GUI::Component::isActive() const
	{
		return isActive_;
	}

	void GUI::Component::activate()
	{
		isActive_ = true;
	}

	void GUI::Component::deactivate()
	{
		isActive_ = false;
	}
}