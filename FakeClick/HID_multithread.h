#pragma once

#include<Windows.h>

namespace FakeClick {
	DWORD WINAPI main_thread(LPVOID input);

	class HID {
		HANDLE m_h_main_thread; //currently not be used
		//HWND h_window;

		const int key_num = 0xFF;
		int m_interval;
		// Index of key_state is the virtual key code
		// reference: https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
		SHORT key_state[256];
		POINT mouse_position;

		friend DWORD WINAPI main_thread(LPVOID input);

		HID(int heartbeat = 10);
		~HID();
	public:
		static HID& get();

		bool b_key_down(int virtual_key_code);
		bool b_key_toggle(int virtual_key_code);

		POINT get_mouse_position(void);
		void get_mouse_position(POINT &position);
	};
}
