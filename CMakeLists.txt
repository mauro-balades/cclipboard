# cclipboard cmake configuration file

cmake_minimum_required(VERSION 2.8)
project(cclipboard)

# Defines and options
set(CCLIPBOARD_VERSION_MAJOR 1 CACHE STRING "cclipboard major version number")
set(CCLIPBOARD_VERSION_MINOR 0 CACHE STRING "cclipboard minor version number")
set(CCLIPBOARD_VERSION "${CCLIPBOARD_VERSION_MAJOR}.${CCLIPBOARD_VERSION_MINOR}" CACHE STRING "cclipboard version number")

option(CCLIPBOARD_FORCE_WIN32 "Force building the Win32 backend (default:off)" OFF)
option(CCLIPBOARD_FORCE_X11 "Force building the X11 backend (default:off)" OFF)
option(CCLIPBOARD_FORCE_COCOA "Force building the Cocoa backend (default:off)" OFF)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" OFF)
set(CCLIPBOARD_BUILD_SHARED ${BUILD_SHARED_LIBS})

if (MINGW)
    option(gtest_disable_pthreads "Disable gtest pthreads (default:on)" ON)
elseif(MSVC)
    option(gtest_force_shared_crt "Always use dynamic runtime library (default:on)" ON)
endif()

# Set compiler flags
if (CMAKE_COMPILER_IS_GNUCC)
    set(GCC_COMPILE_FLAGS "-std=c99 -Wall -pedantic -g")

    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
        set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} -fdiagnostics-color=auto")
    endif()
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${GCC_COMPILE_FLAGS}")
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -g")
endif()

# Include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src/include")

# Configure header
configure_file("src/include/cclipboard-config.h.in" "src/include/cclipboard-config.h")

# Source files
set(HEADERS
    src/include/cclipboard_options.h
    src/include/cclipboard.h
    ${CMAKE_CURRENT_BINARY_DIR}/src/include/cclipboard-config.h
)

set(SOURCE
    src/cclipboard.cc
)

# Set the output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Make the library
add_library(clipboard ${HEADERS} ${SOURCE})
target_link_libraries(clipboard LINK_PRIVATE ${CCLIPBOARD_PRIVATE_LIBS})
if (CCLIPBOARD_ADD_SOVERSION)
    # Not by default because my file system doesn't support symlinks
    set_target_properties(clipboard PROPERTIES SOVERSION ${CCLIPBOARD_VERSION} VERSION ${CCLIPBOARD_VERSION})
endif()

# Build sample executables
# TODO
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/samples)

# Install options
install(TARGETS clipboard
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)

# Uninstall target
# TODO
# configure_file(
#     "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#     "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#     IMMEDIATE @ONLY)

# add_custom_target(uninstall
#     COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)