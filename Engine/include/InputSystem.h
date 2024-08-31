/*************************
 * @file InputSystem.h
 * @author zhuzhile08 (zhuzhile08@gmail.com)
 * 
 * @brief A wrapper around SDL input which polls events
 * 
 * @date 2024-08-31
 * 
 * @copyright Copyright (c) 2022
 *************************/

#pragma once

#include <LSD/Hash.h>
#include <LSD/UnorderedSparseMap.h>

#include <glm/glm.hpp>

namespace esengine {

// keyboard input (scancodes)
enum class KeyType {
	a = 4,
	b = 5,
	c = 6,
	d = 7,
	e = 8,
	f = 9,
	g = 10,
	h = 11,
	i = 12,
	j = 13,
	k = 14,
	l = 15,
	m = 16,
	n = 17,
	o = 18,
	p = 19,
	q = 20,
	r = 21,
	s = 22,
	t = 23,
	u = 24,
	v = 25,
	w = 26,
	x = 27,
	y = 28,
	z = 29,

	b1 = 30,
	b2 = 31,
	b3 = 32,
	b4 = 33,
	b5 = 34,
	b6 = 35,
	b7 = 36,
	b8 = 37,
	b9 = 38,
	b0 = 39,

	kreturn = 40,
	escape = 41,
	backspace = 42,
	tab = 43,
	space = 44,

	minus = 45,
	equals = 46,
	leftbracket = 47,
	rightbracket = 48,
	backslash = 49,
	nonushash = 50,
	semicolon = 51,
	apostrophe = 52,
	grave = 53,
				
	comma = 54,
	period = 55,
	slash = 56,

	capslock = 57,

	f1 = 58,
	f2 = 59,
	f3 = 60,
	f4 = 61,
	f5 = 62,
	f6 = 63,
	f7 = 64,
	f8 = 65,
	f9 = 66,
	f10 = 67,
	f11 = 68,
	f12 = 69,

	printscreen = 70,
	scrolllock = 71,
	pause = 72,
	insert = 73,
	home = 74,
	pageup = 75,
	kdelete = 76,
	end = 77,
	pagedown = 78,
	right = 79,
	left = 80,
	down = 81,
	up = 82,

	numlockclear = 83,
	kpDivide = 84,
	kpMultiply = 85,
	kpMinus = 86,
	kpPlus = 87,
	kpEnter = 88,
	kp1 = 89,
	kp2 = 90,
	kp3 = 91,
	kp4 = 92,
	kp5 = 93,
	kp6 = 94,
	kp7 = 95,
	kp8 = 96,
	kp9 = 97,
	kp0 = 98,
	kpPeriod = 99,

	nonusbackslash = 100,
	application = 101,
	power = 102,
	kpEquals = 103,
	f13 = 104,
	f14 = 105,
	f15 = 106,
	f16 = 107,
	f17 = 108,
	f18 = 109,
	f19 = 110,
	f20 = 111,
	f21 = 112,
	f22 = 113,
	f23 = 114,
	f24 = 115,
	execute = 116,
	help = 117,
	menu = 118,
	select = 119,
	stop = 120,
	again = 121,
	undo = 122,
	cut = 123,
	copy = 124,
	paste = 125,
	find = 126,
	mute = 127,
	volumeup = 128,
	volumedown = 129,
	kpComma = 133,
	kpEqualsas400 = 134,

	international1 = 135,
	international2 = 136,
	international3 = 137,
	international4 = 138,
	international5 = 139,
	international6 = 140,
	international7 = 141,
	international8 = 142,
	international9 = 143,
	lang1 = 144,
	lang2 = 145,
	lang3 = 146,
	lang4 = 147,
	lang5 = 148,
	lang6 = 149,
	lang7 = 150,
	lang8 = 151,
	lang9 = 152,

	alterase = 153,
	sysreq = 154,
	cancel = 155,
	clear = 156,
	prior = 157,
	return2 = 158,
	separator = 159,
	out = 160,
	oper = 161,
	clearagain = 162,
	crsel = 163,
	exsel = 164,

	kp00 = 176,
	kp000 = 177,
	thousandsseparator = 178,
	decimalseparator = 179,
	currencyunit = 180,
	currencysubunit = 181,
	kpLeftparen = 182,
	kpRightparen = 183,
	kpLeftbrace = 184,
	kpRightbrace = 185,
	kpTab = 186,
	kpBackspace = 187,
	kpA = 188,
	kpB = 189,
	kpC = 190,
	kpD = 191,
	kpE = 192,
	kpF = 193,
	kpXor = 194,
	kpPower = 195,
	kpPercent = 196,
	kpLess = 197,
	kpGreater = 198,
	kpAmpersand = 199,
	kpDblampersand = 200,
	kpVerticalbar = 201,
	kpDblverticalbar = 202,
	kpColon = 203,
	kpHash = 204,
	kpSpace = 205,
	kpAt = 206,
	kpExclam = 207,
	kpMemstore = 208,
	kpMemrecall = 209,
	kpMemclear = 210,
	kpMemadd = 211,
	kpMemsubtract = 212,
	kpMemmultiply = 213,
	kpMemdivide = 214,
	kpPlusminus = 215,
	kpClear = 216,
	kpClearentry = 217,
	kpBinary = 218,
	kpOctal = 219,
	kpDecimal = 220,
	kpHexadecimal = 221,

	lctrl = 224,
	lshift = 225,
	lalt = 226,
	lgui = 227,
	rctrl = 228,
	rshift = 229,
	ralt = 230,
	rgui = 231,

	mode = 257,

	audionext = 258,
	audioprev = 259,
	audiostop = 260,
	audioplay = 261,
	audiomute = 262,
	mediaselect = 263,
	www = 264,
	mail = 265,
	calculator = 266,
	computer = 267,
	acSearch = 268,
	acHome = 269,
	acBack = 270,
	acForward = 271,
	acStop = 272,
	acRefresh = 273,
	acBookmarks = 274,

	brightnessdown = 275,
	brightnessup = 276,
	displayswitch = 277,

	kbdillumtoggle = 278,
	kbdillumdown = 279,
	kbdillumup = 280,
	eject = 281,
	sleep = 282,

	app1 = 283,
	app2 = 284,

	audiorewind = 285,
	audiofastforward = 286,

	unknown = 0
};

enum class MouseButtonType {
	left = 1,
	middle = 2,
	right = 4,
	x1 = 8,
	x2 = 16
};

enum class ControllerButtonType {
	invalid = -1,
	a,
	b,
	x,
	y,
	back,
	guide,
	start,
	leftStick,
	rightStick,
	leftShoulder,
	rightShoulder,
	dpadUp,
	dpadDown,
	dpadLeft,
	dpadRight,
	misc1, /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
	paddle1, /* Xbox Elite paddle P1 */
	paddle2, /* Xbox Elite paddle P3 */
	paddle3, /* Xbox Elite paddle P2 */
	paddle4, /* Xbox Elite paddle P4 */
	touchpad, /* PS4/PS5 touchpad button */
};


class Button {
public:
	bool pressed;
	bool released;
	bool held;
	
	void reset() noexcept {
		pressed = false;
		released = false;
		held = false;
	}
};

class Key : public Button {
public:
	Key() noexcept = default;
	Key(KeyType t) noexcept : type(t) { }
	
	KeyType type;
};

class MouseButton : public Button {
public:
	MouseButton() noexcept = default;
	MouseButton(MouseButtonType t) noexcept : type(t) { }
	
	MouseButtonType type;
};

class ControllerButton : public Button {
public:
	ControllerButton() noexcept = default;
	ControllerButton(ControllerButtonType t) noexcept : type(t) { }
	
	ControllerButtonType type;
};


class InputSystem {
public:
	InputSystem();

	bool quit();
	void cancelQuit();

	Key keyboard(KeyType type);
	MouseButton mouse(MouseButtonType type);
	ControllerButton controller(ControllerButtonType type);
	glm::vec2 mousePos();
	glm::vec2 mouseDelta();
	glm::vec2 analogueStickPos();

	void update();

private:
	lsd::UnorderedSparseMap<KeyType, Key> m_keys;
	lsd::UnorderedSparseMap<MouseButtonType, MouseButton> m_mouseButtons;
	lsd::UnorderedSparseMap<ControllerButtonType, ControllerButton> m_controllerButtons;

	bool m_quit = false;

	std::uint32_t m_mouseState;
	const std::uint8_t*m_keyboardState;

	glm::vec2 m_mousePos;
	glm::vec2 m_mouseDelta;
	glm::vec2 m_stickPos;
	glm::vec2 m_stickDelta;
};

} // namespace esengine


namespace lsd {

template <> struct Hash<esengine::Key> {
	constexpr std::size_t operator()(const esengine::Key& k) const noexcept {
		return static_cast<std::size_t>(k.type);
	}
};

template <> struct Hash<esengine::MouseButton> {
	constexpr std::size_t operator()(const esengine::MouseButton& b) const noexcept {
		return static_cast<std::size_t>(b.type);
	}
};

template <> struct Hash<esengine::ControllerButton> {
	constexpr std::size_t operator()(const esengine::ControllerButton& b) const noexcept {
		return static_cast<std::size_t>(b.type);
	}
};

} // namespace lsd
