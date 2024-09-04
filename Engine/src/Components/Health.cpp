
#include <Components/Health.h>

namespace esengine {

Health::Health(std::uint32_t lives, std::uint32_t maxHealth) noexcept : m_lives(lives), m_health(maxHealth), m_maxHealth(maxHealth) { }

bool Health::alive() const noexcept {
	if (m_lives > 0 || m_health > 0) return true;
	else return false;
}

void Health::takeDamage(std::uint32_t damage) noexcept {
	if (m_lives > 0 && m_health < damage) {
		--m_lives;
		m_health = m_maxHealth;
	} else if (m_health > damage) m_health -= damage;
	else m_health = 0;
}

void Health::addLife() noexcept {
	m_health = m_maxHealth;
	m_lives++;
}

} // namespace esengine
