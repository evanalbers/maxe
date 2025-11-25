# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("TheSimulator" VERSION 2.0.0 LANGUAGES CXX)

# C++ standard
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_FRAMEWORK NEVER)
set(Python_FIND_VIRTUALENV ONLY)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Include sub-projects.
add_subdirectory(pybind11)
add_subdirectory ("TheSimulator")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_include_directories(TheSimulator PRIVATE 
    /Users/evaers/Documents/Repos/collusion-theory/maxe/pybind11/include
)
target_link_libraries("TheSimulator" PRIVATE pybind11::embed Threads::Threads)

# To Check if we need filesystem
include(CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator filesystem cxx17fs)

if(NOT cxx17fs)
    target_link_libraries("TheSimulator" PRIVATE stdc++fs)
endif()