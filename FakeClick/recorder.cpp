#include<iostream>

#include"recorder.h"
#include"log.h"

using namespace FakeClick;

Recorder::Recorder()
	:path_ptr(nullptr)
{
}

Recorder& Recorder::get()
{
	static Recorder instance;
	return instance;
}

bool Recorder::record()
{
	if (path_ptr == nullptr)
		return false;
	LOG("start record");

	Frame tmp_frame;
	bool prev_lbutton_state = HID::get().b_key_down(VK_LBUTTON);
	bool prev_rbutton_state = HID::get().b_key_down(VK_RBUTTON);

	while (!(HID::get().b_key_down(VK_SPACE))) {
		bool now_lbutton_state = HID::get().b_key_down(VK_LBUTTON);
		bool now_rbutton_state = HID::get().b_key_down(VK_RBUTTON);
		tmp_frame.state = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		if (now_lbutton_state != prev_lbutton_state) {
			if (now_lbutton_state)
				tmp_frame.state |= MOUSEEVENTF_LEFTDOWN;
			else
				tmp_frame.state |= MOUSEEVENTF_LEFTUP;
		}

		if (now_rbutton_state != prev_rbutton_state) {
			if (now_rbutton_state)
				tmp_frame.state |= MOUSEEVENTF_RIGHTDOWN;
			else
				tmp_frame.state |= MOUSEEVENTF_RIGHTUP;
		}
		prev_lbutton_state = now_lbutton_state;
		prev_rbutton_state = now_rbutton_state;

		HID::get().get_mouse_position(tmp_frame.position);
		// Thanks, Windows
		// In MOUSE_INPUT fucking structure, 
		// the screen is mapped to a scale from 0 to 65535 in both axis.
		tmp_frame.position.x = ((tmp_frame.position.x << 16) - 1) / GetSystemMetrics(SM_CXSCREEN);
		tmp_frame.position.y = ((tmp_frame.position.y << 16) - 1) / GetSystemMetrics(SM_CYSCREEN);
		path_ptr->frames.push_back(tmp_frame);
		Sleep(heartbeat);
	}
	LOG("stop record");
	return true;
}

bool Recorder::load(Path &path)
{
	if (path_ptr != nullptr)
		return false;
	path_ptr = &path;
	LOG("Recoder::path loaded");
	return true;
}

Path& Recorder::unload()
{
	Path *tmp = path_ptr;
	path_ptr = nullptr;
	return *tmp;
}

bool Recorder::empty()
{
	return (path_ptr == nullptr);
}
