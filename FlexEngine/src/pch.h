#pragma once

// https://en.cppreference.com/w/cpp/header

#include <cassert>                    // Conditionally compiled macro that compares its argument to zero

//#include <iostream>                   // Several standard stream objects
//#include <string>                     // std::basic_string class template
#include <sstream>                    // std::basic_stringstream, std::basic_istringstream, std::basic_ostringstream class templates and several typedefs

#include <array>                      // std::array container
#include <vector>                     // std::vector container
#include <unordered_map>              // std::unordered_map and std::unordered_multimap unordered associative containers
#include <unordered_set>              // std::unordered_set and std::unordered_multiset unordered associative containers

#include <memory>                     // High-level memory management utilities
//#include <utility>                    // Various utility components
#include <algorithm>                  // Algorithms that operate on ranges
#include <functional>                 // Function objects, Function invocations, Bind operations and Reference wrappers

#include "FlexEngine/timer.h"         // Timer class (includes <iostream> and <chrono>)
#include "FlexEngine/flexlogger.h"    // Log class (includes <filesystem>) (dependancy includes <fstream> and <string>)
