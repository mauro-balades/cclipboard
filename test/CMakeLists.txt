# Source files
set (SOURCE
    test_basics.cpp
)

# Add the target
add_executable (run-tests ${HEADERS} ${SOURCE})

# Link it to gtest
target_link_libraries(run-tests LINK_PRIVATE gtest gtest_main)

if (BUILD_SHARED_LIBS)
    target_compile_definitions(run-tests PRIVATE -DGTEST_LINKED_AS_SHARED_LIBRARY)
endif()

# Link it with cclipboard
target_link_libraries (run-tests LINK_PUBLIC clipboard)

# For `make test`
add_test(NAME cclipboard-testing
         COMMAND run-tests --gtest_color=yes
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

