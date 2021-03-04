cmake_minimum_required(VERSION 3.13)
project(net_simulation)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Werror=switch)
include_directories(include mocks)
set(PROJECT_ID net_simulation)

set(SOURCE_FILES
        src/package.cpp
        src/types.cpp
        src/helpers.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/factory.cpp
        src/reports.cpp
        src/simulation.cpp)

set(EXEC_DEBUG ${PROJECT_ID}__debug)

#add_executable(tsp ${EXEC_DEBUG} ${SOURCE_FILES} main.cpp)
add_executable(${EXEC_DEBUG} ${SOURCE_FILES} main.cpp)

# == Unit testing using Google Testing Framework ==
set(SOURCES_FILES_TESTS
        test/test_package_nasze.cpp
        test/test_storage_types_nasze.cpp
        test/test_nodes_nasze.cpp
        test/test_factory_nasze.cpp

        test/test_package.cpp
        test/test_storage_types.cpp
        test/test_nodes.cpp
        test/test_Factory.cpp
        test/test_factory.cpp
        test/test_reports.cpp
        test/test_simulate.cpp
        test/test_factory_io.cpp
        )

set(EXEC_TEST ${PROJECT_ID}__test)
add_executable(${EXEC_TEST} ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)
target_compile_definitions(${EXEC_TEST} PUBLIC EXERCISE_ID=SIMULATION)
set(GTEST_ROOT googletest-master)
target_include_directories(${EXEC_TEST} PUBLIC
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
        )
add_subdirectory(${GTEST_ROOT} googletest-master)
target_link_libraries(${EXEC_TEST} gmock)

