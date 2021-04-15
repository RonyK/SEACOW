//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <bitset>
#include <memory>
#include <tuple>
#include <string>
#include <cstdlib>
#include "gtest/gtest.h"

#define _pFuncGetSourceArray_		void(*getSourceArrayIfEmpty)(std::vector<pArray>&)
#define _pFuncGetSourceArrayDesc_	void(*getSourceArrayDesc)(std::vector<pArray>&)
#define _vectorSourceArray_			std::vector<pArray>& sourceArr

#include <dummy/dummy.h>
#include <dummy/opDummy.h>
#include <dummy/opArrayDummy.h>
#include <dummy/opPlanDummy.h>
#include <dummy/dummy_astronomy.h>

static const int se_array_id = 2;
static const int delta_spiht_array_id = 3;
static const int spiht_array_id = 4;
static const int compass_array_id = 5;
static const int zip_array_id = 6;

static const int rangeSeed = 2102;
static const int filterValueSeed = 210205;
