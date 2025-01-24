# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("TheSimulator" VERSION 2.0.0 LANGUAGES CXX)

# C++ standard
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 17)

# trying to find venv 
set(CMAKE_FIND_FRAMEWORK NEVER)
set(Python_FIND_VIRTUALENV ONLY)

# Getting parent directory (market-sim)
set(MARKET_SIM "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Copying files from agent code to maxe/build/TheSimulator/TheSimulator
file(GLOB AGENT_CODE "${CMAKE_CURRENT_SOURCE_DIR}/../agent code/*")

# Copying files from simulations folder to maxe/build/TheSimulator/TheSimulator
file(GLOB SIMULATIONS "${CMAKE_CURRENT_SOURCE_DIR}/../simulations/*")

# Variable to store names of agent code files, simulation files
set(AGENT_FILE_NAMES)
set(SIMULATION_FILE_NAMES)

# for each agent code file
foreach(FILE_PATH ${AGENT_CODE})
	
	# Extracting Filename
	get_filename_component(FILE_NAME "${FILE_PATH}" NAME)

	# Append to list of filenames 
	list(APPEND AGENT_FILE_NAMES ${FILE_NAME})

endforeach()

# for each simulation file
foreach(FILE_PATH ${SIMULATIONS})

	# extracting filenames
	get_filename_component(FILE_NAME "${FILE_PATH}" NAME)
	
	# appending to list of simulation file names 
	list(APPEND SIMULATION_FILE_NAMES ${FILE_NAME})
endforeach()

# copying agent code files to destination directory
foreach(FILE ${AGENT_CODE})
	file(COPY ${FILE} DESTINATION ${MARKET_SIM}/maxe/build/TheSimulator/TheSimulator)
endforeach()

# copying simulation code files to destination directory
foreach(FILE ${SIMULATIONS})
	file(COPY ${FILE} DESTINATION ${MARKET_SIM}/maxe/build/TheSimulator/TheSimulator)
endforeach()




# joining agent file names, simulation file names into one line, printing 
string(JOIN " " ALL_SIMULATION_FILE_NAMES ${SIMULATION_FILE_NAMES})
string(JOIN " " ALL_AGENT_FILE_NAMES ${AGENT_FILE_NAMES})
message(STATUS "Found the following agent code files: ${ALL_AGENT_FILE_NAMES}")
message(STATUS "Found the following simulation files: ${ALL_SIMULATION_FILE_NAMES}")

# Include sub-projects.
add_subdirectory(pybind11)
add_subdirectory ("TheSimulator")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries("TheSimulator" PRIVATE pybind11::embed Threads::Threads)

# To Check if we need filesystem
include(CheckCXXSymbolExists)
CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator filesystem cxx17fs)

if(NOT cxx17fs)
    target_link_libraries("TheSimulator" PRIVATE stdc++fs)
endif()
