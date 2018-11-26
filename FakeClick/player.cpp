#include"player.h"
#include"HID_multithread.h"
#include"log.h"

using namespace FakeClick;

Player::Player()
	:path_ptr(nullptr)
{
}

Player& Player::get()
{
	static Player instance;
	return instance;
}

bool Player::play()
{
	INPUT mouse_input;
	ZeroMemory(&mouse_input, sizeof(INPUT));
	mouse_input.type = INPUT_MOUSE;
	mouse_input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	mouse_input.mi.dwExtraInfo = NULL;
	mouse_input.mi.time = 0;
	LOG("start play");
	for (Vector<Frame>::iterator it = path_ptr->frames.begin();
		 it != path_ptr->frames.end();
		 ++it) {
		if (HID::get().b_key_down(VK_SPACE))
			return false;
		// SetCursorPos(it->position.x, it->position.y);
		// Windows stupid coordinates
		mouse_input.mi.dx = it->position.x;
		mouse_input.mi.dy = it->position.y;
		mouse_input.mi.dwFlags = it->state;
		SendInput(1, &mouse_input, sizeof(INPUT));
		Sleep(heartbeat);
	}
	LOG("played all");
	return true;
}

bool Player::load(Path &path)
{
	// If last path not unloaded, return false.
	if (path_ptr != nullptr)
		return false;
	path_ptr = &path;
	LOG("Player:: path loaded");
	return true;
}

Path& Player::unload()
{
	Path *tmp = path_ptr;
	path_ptr = nullptr;
	return *tmp;
}

bool Player::empty()
{
	return (path_ptr == nullptr);
}
