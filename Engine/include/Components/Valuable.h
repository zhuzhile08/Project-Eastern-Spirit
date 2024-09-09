/*************************
 * @file Valuable.h
 * @author Okeeee_Holz
 *
 * @brief Health system
 *
 * @date 2024-09-07
 *
 * @copyright Copyright (c) 2024
 *************************/

#pragma once

#include <cstddef>

namespace esengine {

class Valuable {
public:
	Valuable(std::uint32_t value) : m_value(value) noexcept { }

	[[nodiscard]] std::uint32_t value() const noexcept {
		return m_value;
	}

private:
	std::uint32_t m_value;
};

} //namespace esengine
