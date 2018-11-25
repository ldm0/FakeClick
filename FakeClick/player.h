#pragma once

#include"path.h"

using namespace FakeClick;

// Singleton
class Player {
	const int heartbeat = 10;
	Path *path_ptr;
public:
	Player();
	static Player& get();

	// The member is a pointer,
	// so its possible that the path accidently be released
	// like the tape suddenly break. If this happened,
	// the play function return false;
	bool play();

	// If another path is not unloaded, 
	// the new path cannot be load.
	bool load(Path &path);
	Path& unload();
	bool empty();
};