#include <iostream>
#include "bitstringstream.h"

namespace caWavelet
{
	const unsigned char obitstream::mask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};

	_NODISCARD _MRTIMP2 _BitSmanip<std::streamsize> __cdecl setw(std::streamsize w)
	{
		auto h = [](obitstream& _oStr, std::streamsize w)->obitstream& {_oStr.width(w); return _oStr; };
		return _BitSmanip<std::streamsize>(h, w);
	}
}