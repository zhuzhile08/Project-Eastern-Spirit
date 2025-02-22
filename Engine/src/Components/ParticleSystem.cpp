#include <Components/ParticleSystem.h>

#include <Detail/RenderSystem.h>
#include <Graphics/Texture.h>

namespace esengine {

void ParticleSystem::update(ms_time_t deltaTime) {
	(*m_spawnSystem)(m_particles, m_time, deltaTime);

	auto it = m_particles.begin();
	while (it != m_particles.end()) {
		if (!(*m_updateSystem)(*it, deltaTime))
			it = m_particles.erase(it);
		else ++it;
	}
}

void ParticleSystem::draw(
	const etcs::Entity& entity, 
	const etcs::Transform& transform,
	const glm::mat4& renderMatrix
) const {
	auto tfBase = renderMatrix * glm::vec4(transform.globalTranslation(entity), 1.0f);
	auto rotBase = transform.globalOrientation(entity).z * -360;

	auto& drawCalls = globals::renderSystem->drawData(static_cast<int>(tfBase.z));

	auto tx = m_texture->texture();

	for (const auto& particle : m_particles) {
		auto tf = tfBase + glm::vec4(particle.offset, 0.0f, 0.0f);
		auto scale = glm::vec2(particle.rect.w, particle.rect.h) * particle.scale;

		drawCalls.emplaceBack(
			rotBase + particle.rotation,
			particle.rect,
			SDL_FRect {
				tf.x,
				tf.y,
				scale.x,
				scale.y
			},
			tx
		);
	}
}

} // namespace esengine
