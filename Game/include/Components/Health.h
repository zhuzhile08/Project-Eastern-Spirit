/*************************
 * @file Health.h
 * @author Okeeee_Holz
 *
 * @brief Health system
 *
 * @date 2024-09-03
 *
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <cstdint>

namespace esengine {

class Health {
public:
	Health(std::uint32_t lives, std::uint32_t maxHealth) noexcept;

	void takeDamage(std::uint32_t damage) noexcept;
	void addLife() noexcept;
	bool alive() const noexcept;

	[[nodiscard]] std::uint32_t lives() const noexcept {
		return m_lives;
	}
	[[nodiscard]] std::uint32_t health() const noexcept {
		return m_health;
	}

private:
	std::uint32_t m_lives;
	std::uint32_t m_health;
	const std::uint32_t m_maxHealth;
};

} // namespace esengine
