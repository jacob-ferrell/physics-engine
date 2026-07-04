cmake_minimum_required(VERSION 3.20)
project(physics_engine LANGUAGES CXX)

# ---- Global settings ----------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Always emit compile_commands.json for clangd / editor tooling.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release if the caller didn't pick a build type.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- Sanitizer selection -------------------------------------------------
# PHYS_SANITIZER = OFF | address | thread. The scripts in ./scripts flip this.
set(PHYS_SANITIZER "OFF" CACHE STRING "Sanitizer to enable: OFF, address, thread")
set_property(CACHE PHYS_SANITIZER PROPERTY STRINGS OFF address thread)

# INTERFACE target: a named bag of flags every target links. Warnings, the
# sanitizer flags, and the include/ path all live here in one place.
add_library(phys_flags INTERFACE)
target_compile_options(phys_flags INTERFACE
  -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
target_include_directories(phys_flags INTERFACE include)

if(PHYS_SANITIZER STREQUAL "address")
  message(STATUS "Sanitizer: AddressSanitizer + UndefinedBehaviorSanitizer")
  target_compile_options(phys_flags INTERFACE
    -fsanitize=address,undefined -fno-omit-frame-pointer -g)
  target_link_options(phys_flags INTERFACE -fsanitize=address,undefined)
elseif(PHYS_SANITIZER STREQUAL "thread")
  message(STATUS "Sanitizer: ThreadSanitizer")
  target_compile_options(phys_flags INTERFACE
    -fsanitize=thread -fno-omit-frame-pointer -g)
  target_link_options(phys_flags INTERFACE -fsanitize=thread)
elseif(NOT PHYS_SANITIZER STREQUAL "OFF")
  message(FATAL_ERROR "Unknown PHYS_SANITIZER='${PHYS_SANITIZER}' (use OFF, address, or thread)")
endif()

# ---- Targets (materialize as you add source files) -----------------------
# The tree starts empty on purpose — you write the engine. These blocks stay
# dormant until the matching files exist, so `cmake` configures cleanly now
# and starts building things the moment you drop sources in.
#
# CONFIGURE_DEPENDS re-scans these globs on every build, so newly added files
# are picked up without you re-running cmake by hand.

# Engine library = everything in src/ except main.cpp.
file(GLOB PHYS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(FILTER PHYS_SOURCES EXCLUDE REGEX "main\\.cpp$")
if(PHYS_SOURCES)
  add_library(physics ${PHYS_SOURCES})
  target_link_libraries(physics PUBLIC phys_flags)
else()
  message(STATUS "physics library: no sources in src/ yet")
endif()

# Demo executable, built from src/main.cpp when it exists.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
  add_executable(physics_demo src/main.cpp)
  if(TARGET physics)
    target_link_libraries(physics_demo PRIVATE physics)
  else()
    target_link_libraries(physics_demo PRIVATE phys_flags)
  endif()
endif()

# Tests: any *.cpp under tests/ becomes the test runner, wired into CTest.
enable_testing()
file(GLOB PHYS_TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
if(PHYS_TEST_SOURCES)
  add_executable(physics_tests ${PHYS_TEST_SOURCES})
  if(TARGET physics)
    target_link_libraries(physics_tests PRIVATE physics)
  else()
    target_link_libraries(physics_tests PRIVATE phys_flags)
  endif()
  add_test(NAME physics_tests COMMAND physics_tests)
endif()
