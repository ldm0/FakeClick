#include"HID_multithread.h"

#include<iostream>

using namespace FakeClick;

//C style function, so use ExitThread is just fine
//No class member is tolorated!
DWORD WINAPI FakeClick::main_thread(LPVOID input)
{
	HID &instance = *(HID *)input;
	//MSG message;
	for (;;) {
		// Get the mouse position
		// For extreme performance and for future multi hit things
		// don't adapt the get-on-request design any more
		GetCursorPos(&(instance.mouse_position));

		//Check the message to check if specific keys down
		//int i = PeekMessageW(&message, instance.h_window, 0, 0, PM_NOREMOVE);
			//std::cout << message.time << std::endl;
		// Don't use message to get input anymore
		//instance.w_down = !!(GetKeyState('W') & 0x8000);
		//instance.a_down = !!(GetKeyState('A') & 0x8000);
		//instance.s_down = !!(GetKeyState('S') & 0x8000);
		//instance.d_down = !!(GetKeyState('D') & 0x8000);
		for (int i = 0; i < instance.key_num; ++i)
			instance.key_state[i] = GetKeyState(i);

		Sleep(instance.m_interval);
	}
	return 0;
}

HID::HID(int heartbeat)
	:m_interval(heartbeat)
{
	for (int i = 0; i < key_num; ++i)
		key_state[i] = 0;
	GetCursorPos(&mouse_position);

	//create a thread to detect the input
	m_h_main_thread = CreateThread(NULL, 0, main_thread, this, 0, NULL);
}

HID::~HID()
{
	if (m_h_main_thread)
		// Yes, I know it's stupid.
		TerminateThread(m_h_main_thread, 0);
}

HID & HID::get()
{
	static HID instance;
	return instance;
}

bool HID::b_key_down(int virtual_key_code)
{
	if (!!(virtual_key_code & (~0xff)))
		__debugbreak();
	return !!(key_state[virtual_key_code] & 0x8000);
}

POINT HID::get_mouse_position(void)
{
	//GetCursorPos(&mouse_position);
	return mouse_position;
}

void HID::get_mouse_position(POINT &position)
{
	//GetCursorPos(&mouse_position);
	position.x = mouse_position.x;
	position.y = mouse_position.y;
}

