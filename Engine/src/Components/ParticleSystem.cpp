#include <Components/ParticleSystem.h>

#include <Detail/RenderSystem.h>
#include <Graphics/Texture.h>

namespace esengine {

void ParticleSystem::update(es_time_t deltaTime) {
	(*m_spawnSystem)(m_particles, m_time, deltaTime);

	auto it = m_particles.begin();
	while (it != m_particles.end()) {
		if (!(*m_updateSystem)(*it, deltaTime))
			it = m_particles.erase(it);
		else ++it;
	}
}

void ParticleSystem::draw(const etcs::Entity& entity, const etcs::Transform& transform, const glm::mat4& camTransform, const Camera& camera) const {
	auto tfBase = camTransform * glm::vec4(transform.globalTranslation(entity), 1.0f);
	auto rotBase = transform.globalOrientation(entity).z * -360;
	auto tx = m_texture->texture(camera.passName());

	auto& renderPass = globals::renderSystem->pass(camera.passName());
	auto& drawCalls = renderPass.drawData[static_cast<int>(tfBase.z * renderPass.sortingFactor)];
	
	for (const auto& particle : m_particles) {
		auto tf = tfBase + glm::vec4(particle.offset, 0.0f, 0.0f);

		if (tf.x >= -particle.rect.w && tf.x < camera.viewport.w && tf.y >= -particle.rect.h && tf.y < camera.viewport.h) {
			auto scale = glm::vec2(particle.rect.w, particle.rect.h) * particle.scale;

			drawCalls.emplaceBack(
				0.0f, // not needed
				rotBase + particle.rotation,
				particle.rect,
				SDL_FRect {
					tf.x + camera.viewport.x - scale.x / 2,
					tf.y + camera.viewport.y - scale.y / 2,
					scale.x,
					scale.y
				},
				tx
			);
		}
	}
}

} // namespace esengine
