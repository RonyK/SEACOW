#pragma once
#include <iostream>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <type_traits>
#include <algorithm>

// For bitstream
#include <bitset>

// For file system
#include <iosfwd>
#include <fstream>
#include <filesystem>
#include <memory>

// For timer
#include <chrono>
#include <mutex>

////////////////////////////////////////
// BOOST LIBRARY
//
// For value
#include <boost/any.hpp>

// For timmer
#include <boost/format.hpp>

// For logger
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <xml/tinyxml2.h>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;