#pragma once
#include<Windows.h>
#include"string.h"
#include"vector.h"

namespace FakeClick {
	// The Frame struct will be optimized later
	// Firstly create a runable version XD
	struct Frame {
		int state;
		// time gap between two frames is equal, so no time_stamp
		// time_t time_stamp;
		POINT position;
	};

	struct Path {
		FakeClick::Vector<Frame> frames;
	};
}