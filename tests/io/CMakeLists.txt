cmake_minimum_required(VERSION 3.13...3.25)

set(TESTNAME io_test)

add_executable(${TESTNAME} ${TESTNAME}.cc)

target_link_libraries(${TESTNAME}
    PRIVATE
        gtest_main
        io
)

gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
)

set_target_properties(${TESTNAME}
    PROPERTIES FOLDER tests
)
