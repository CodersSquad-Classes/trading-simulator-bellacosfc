# Set the minimum required version of cmake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(clob_engine)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(clob_engine
    main.cpp
    OrderBook.cpp
    Dashboard.cpp
)

# Link threads library (for std::this_thread)
find_package(Threads REQUIRED)
target_link_libraries(clob_engine PRIVATE Threads::Threads)

# Optional: Enable compiler warnings for better code
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Tell CMake where to find headers (the current directory)
target_include_directories(clob_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})