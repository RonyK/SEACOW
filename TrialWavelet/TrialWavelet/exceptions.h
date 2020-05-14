#pragma once
#include <iostream>

namespace caWavelet
{
    class NotImplemented : public std::logic_error
    {
    public:
        NotImplemented() : std::logic_error("Function is not implemented") { };
    };

    class BitstreamIOException : public std::logic_error
    {
    public:
        BitstreamIOException() : std::logic_error("IO Exception in bitstream") {};
    };
}
