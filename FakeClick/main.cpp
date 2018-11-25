#include<iostream>
#include"HID_multithread.h"
#include"player.h"
#include"recorder.h"
#include"string.h"

using namespace FakeClick;

int main()
{
	Path path;
	Recorder::get().load(path);
	std::cin.get();
	Recorder::get().record();
	std::cin.get();
	Player::get().load(path);
	std::cin.get();
	Player::get().play();
}