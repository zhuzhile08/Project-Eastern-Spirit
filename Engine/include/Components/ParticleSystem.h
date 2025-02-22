/*************************
 * @file ParticleSystem.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief Particle system implementation
 * 
 * @date 2024-11-25
 * @copyright Copyright (c) 2024
 **************************/

#pragma once

#include <Core/Common.h>
#include <Core/Math.h>

#include <Core/Animation.h>

#include <Components/Camera.h>

#include <ETCS/Entity.h>
#include <ETCS/Components/Transform.h>

#include <LSD/Vector.h>
#include <LSD/FunctionPointer.h>

#include <glm/glm.hpp>

#include <cstddef>

namespace esengine {

class ParticleSystem {
public:
	class Particle {
	public:
		glm::vec2 offset;
		glm::vec2 scale;
		float rotation;

		SDL_FRect rect;

		bool visible = true;

		ms_time_t timeAlive = 0;
	};

	typedef bool (*update_func)(Particle&, ms_time_t);
	typedef void (*spawner_func)(lsd::Vector<Particle>, ms_time_t&, ms_time_t);


	ParticleSystem(Texture* texture, update_func updateSystem) : m_texture(texture), m_updateSystem(updateSystem) { }

	void update(ms_time_t deltaTime);
	void draw(const etcs::Entity& entity, const etcs::Transform& transform, const glm::mat4& renderMatrix) const;

private:
	lsd::Vector<Particle> m_particles;
	Texture* m_texture;

	update_func m_updateSystem;

	ms_time_t m_time;
	spawner_func m_spawnSystem;
};

} // namespace esengine
