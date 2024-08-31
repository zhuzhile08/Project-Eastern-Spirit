/*************************
 * @file RAIIContainers.h
 * @author Zhile Zhu (zhuzhile08@gmail.com)
 * 
 * @brief A RAII container template for storing and managing handles
 * 
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2024
 ************************/

#pragma once

#include <SDL3/SDL.h>

#include <LSD/StringView.h>

#include <type_traits>
#include <algorithm>

namespace esengine {

namespace detail {

template <class Ty> concept RAIIContainerType = 
	std::is_copy_constructible_v<Ty> && 
	std::is_copy_assignable_v<Ty> && 
	std::is_move_constructible_v<Ty> &&
	std::is_move_assignable_v<Ty>;

template <RAIIContainerType Ty> class RAIIContainer {
public:
	using handle_type = Ty;
	using const_handle = const handle_type;
	using handle_reference = Ty&;
	using const_handle_reference = const_handle&;

	using container = RAIIContainer;
	using movable_container = container&&;

	constexpr RAIIContainer() noexcept = default;
	constexpr RAIIContainer(movable_container container) noexcept : 
		m_handle(std::exchange(container.m_handle, handle_type { } )) { }

	constexpr RAIIContainer(lsd::StringView title, int x, int y, int flags) 
		requires(std::is_same_v<handle_type, SDL_Window*>) {
		if (!m_handle) {
			m_handle = SDL_CreateWindow(title.data(), x, y, flags);
		}
	}

	void destroy() {
		if (m_handle) {
			if constexpr (std::same_as<handle_type, SDL_Window*>) {
				SDL_DestroyWindow(m_handle);
			}
		}

		m_handle = nullptr;
	}
	virtual ~RAIIContainer() {
		destroy();
	}

	constexpr RAIIContainer& operator=(movable_container other) noexcept {
		std::swap(m_handle, other.m_handle);
		return *this;
	}

	[[nodiscard]] constexpr handle_type release() noexcept {
		return std::exchange(m_handle, handle_type { });
	}

	constexpr void swap(container& other) noexcept {
		std::swap(m_handle, other.m_handle);
	}

	constexpr handle_type& get() noexcept { return m_handle; }
	constexpr const_handle& get() const noexcept { return m_handle; }
	constexpr operator handle_type&() noexcept { return m_handle; }
	constexpr operator const_handle&() const noexcept { return m_handle; }

private:
	handle_type m_handle { };
};

} // namespace detail

namespace sdl {

using Window = detail::RAIIContainer<SDL_Window*>;
using Renderer = detail::RAIIContainer<SDL_Renderer*>;
using Texture = detail::RAIIContainer<SDL_Texture*>;

}

} // namespace esengine
