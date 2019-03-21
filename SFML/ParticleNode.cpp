/**
* @file
* @author
* Justin Lange 2018
* @version 1.0
*
*
* @section DESCRIPTION
* ParticleNode Class
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

#include "ParticleNode.h"
#include "DataTables.h"

namespace GEX
{ 
	namespace
	{
		const std::map<Particle::Type, ParticleData> TABLE = initializeParticleData();
	}

	ParticleNode::ParticleNode(Particle::Type type, const TextureManager& textures)
		: SceneNode()
		, particles_()
		, texture_(textures.get(GEX::TextureID::Particle))
		, type_(type)
		, vertexArray_(sf::Quads)
		, needsVertexUpdate_(true)
	{}

	void ParticleNode::addParticle(sf::Vector2f position)
	{
		Particle particle;

		particle.position = position;
		particle.color = TABLE.at(type_).color;
		particle.lifetime = TABLE.at(type_).lifetime;

		particles_.push_back(particle);

	}

	Particle::Type ParticleNode::getParticle() const
	{
		return type_;
	}

	unsigned int ParticleNode::getCategory() const
	{
		return Category::ParticleSystem;
	}

	void ParticleNode::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
		// Remove aged out particles
		while (!particles_.empty() && particles_.front().lifetime <= sf::Time::Zero)
			particles_.pop_front();

		// Count down the particles' lifetime
		for (Particle& p : particles_)
			p.lifetime -= dt;

		// Mark for update
		needsVertexUpdate_ = true;
	}

	void ParticleNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		if (needsVertexUpdate_)
		{
			computeVertices();
			needsVertexUpdate_ = false;
		}
		
		states.texture = &texture_;

		// Draw all the vertices
		target.draw(vertexArray_, states);
	}

	void ParticleNode::addVertex(float worldX, float worldY, float texCoordU, float texCoordV, const sf::Color color) const
	{
		sf::Vertex vertex;

		vertex.position = sf::Vector2f(worldX, worldY);
		vertex.texCoords = sf::Vector2f(texCoordU, texCoordV);
		vertex.color = color;

		vertexArray_.append(vertex);
	}

	void ParticleNode::computeVertices() const
	{
		sf::Vector2f size(texture_.getSize());
		sf::Vector2f half = size / 2.f;

		// Refill vertex array
		vertexArray_.clear();
		for (const Particle& p : particles_)
		{
			sf::Vector2f pos = p.position;
			sf::Color color = p.color;

			float ratio = p.lifetime.asSeconds() / TABLE.at(type_).lifetime.asSeconds();
			color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

			addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
			addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
			addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
			addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
		}
	}
}