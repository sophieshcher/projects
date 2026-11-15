cmake_minimum_required(VERSION 3.16)
project(CMSDimuonInvariantMass LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate CERN ROOT installation
find_package(ROOT REQUIRED COMPONENTS RIO Tree Hist Graf Gpad MathCore Physics RooFit RooFitCore)

include(${ROOT_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(analyze_dimuon src/analyze_dimuon.cpp)
target_link_libraries(analyze_dimuon PRIVATE ${ROOT_LIBRARIES})

# Enable compiler optimizations
if(MSVC)
    target_compile_options(analyze_dimuon PRIVATE /O2 /W3)
else()
    target_compile_options(analyze_dimuon PRIVATE -O3 -Wall -Wextra)
endif()
