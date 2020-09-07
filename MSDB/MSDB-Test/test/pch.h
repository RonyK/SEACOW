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
#include "gtest/gtest.h"
#include "dummy/dummy.h"
#include "dummy/opDummy.h"
#include "dummy/dummy_astronomy.h"

#define _pFuncGetSourceArray_ void(*getSourceArrayIfEmpty)(std::vector<pArray>&)
