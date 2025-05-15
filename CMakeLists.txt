cmake_minimum_required(VERSION 3.30)
project(DiscreteTimeMarkovChains)

set(CMAKE_CXX_STANDARD 20)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Source files
file(GLOB SOURCES
        "*.cpp"
)

add_executable(DiscreteTimeMarkovChains ${SOURCES})


