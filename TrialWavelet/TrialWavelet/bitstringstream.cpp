#include <iostream>
#include "bitstringstream.h"

namespace caWavelet
{
	const unsigned char bitstream::mask[7] = { 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
}