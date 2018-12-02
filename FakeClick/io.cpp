#include"io.h"

using namespace FakeClick;

IO & FakeClick::IO::get()
{
	static IO instance;
	return instance;
}

bool FakeClick::IO::file2str(const String file_path, String & des)
{
	return true;
}
