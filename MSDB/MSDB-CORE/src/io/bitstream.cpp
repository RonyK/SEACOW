#include <io/bitstream.h>

namespace msdb
{
	const unsigned char iobs_base::mask[9] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
	const unsigned char iobs_base::rmask[9] = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };

	_MRTIMP2 _BitSmanip<std::streamsize> __cdecl setw(std::streamsize w)
	{
		auto h = [](iobs_base& _oStr, std::streamsize w)->iobs_base& { _oStr.width(w); return _oStr; };
		return _BitSmanip<std::streamsize>(h, w);
	}
}
