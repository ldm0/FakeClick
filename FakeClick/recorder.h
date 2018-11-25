#pragma once

#include"HID_multithread.h"
#include"path.h"

using namespace FakeClick;

// Singleton
class Recorder {
	const int heartbeat = 10;
	Path *path_ptr;
public:
	Recorder();
	static Recorder& get();

	// Like the player, The member is a pointer,
	// so its possible that the path accidently be released.
	// Like the tape suddenly break. If this happened,
	// the record function return false.
	bool record();

	// If another path is not unloaded, 
	// the new path cannot be load successfully.
	bool load(Path &path);
	Path& unload();
	bool empty();
};