#pragma once
#ifndef _caCompact_
#define _caCompact_

#include <iostream>
#include "bitstringstream.h"

namespace caWavelet
{
	template <typename Dty_, typename Ty_>
	class caCompact
	{
		using size_type = size_t;

		using dim_type = Dty_;
		using dim_pointer = Dty_*;
		using dim_const_pointer = const Dty_*;
		using dim_reference = Dty_&;
		using dim_const_rfeference = const Dty_&;

		using dim_vector = std::vector<Dty_>;
		using dim_vector_const_reference = const std::vector<Dty_>&;

		using value_type = Ty_;
		using value_pointer = Ty_*;
		using value_const_pointer = const Ty_*;
		using value_reference = Ty_&;
		using value_const_reference = const Ty_&;

	public:
		caCompact() {}

		int encode(bstream& output, dim_const_pointer wtData, dim_vector_const_reference dims, size_type wtLevel)
		{
			this->data = wtData;
			this->dims = dims;
			this->wtLevel = wtLevel;
		}

	protected:
		dim_const_pointer data;
		dim_vector dims;
		size_type wtLevel;

	};
}

#endif	// _caCompact_