#pragma once

// FlexEngine: Standard Library Header File
// https://en.cppreference.com/w/cpp/header

//#include <cassert>                    // Conditionally compiled macro that compares its argument to zero

//#include <iostream>                   // Several standard stream objects
#include <fstream>                    // Input/output stream class to operate on files
//#include <string>                     // std::basic_string class template
#include <sstream>                    // std::basic_stringstream, std::basic_istringstream, std::basic_ostringstream class templates and several typedefs

#include <array>                      // std::array container
#include <vector>                     // std::vector container
#include <unordered_map>              // std::unordered_map and std::unordered_multimap unordered associative containers
#include <unordered_set>              // std::unordered_set and std::unordered_multiset unordered associative containers

#include <memory>                     // High-level memory management utilities
//#include <utility>                    // Various utility components
#include <algorithm>                  // Algorithms that operate on ranges
//#include <functional>                 // Function objects, Function invocations, Bind operations and Reference wrappers

#include "FlexEngine/flexassert.h"    // FLX_INTERNAL_ASSERT, FLX_CORE_ASSERT, FLX_ASSERT, FLX_NULLPTR_ASSERT
                                      // (includes <iostream>, "flexlogger.h", and "freequeue.h")
                                      // (dependancy includes "files.h", <filesystem>, <fstream>, <string>, <deque>, and <functional>)
#include "FlexEngine/timer.h"         // Timer class (includes <iostream> and <chrono>)
