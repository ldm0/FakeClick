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

	bool record();

	// If another path is not unloaded, 
	// the new path cannot be load successfully.
	bool load(Path &path);
	Path& unload();
	bool empty();
};