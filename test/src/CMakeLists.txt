include_directories(${gtest_SOURCE_DIR}/include)
include_directories(${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include)
include_directories(${gmock_SOURCE_DIR})
include_directories(${qc_BUILD_DIR}/src/parse)

add_definitions(-DQC_TEST)

add_executable(qc_tests
        det_parser/det_parser_test.cpp
        collectors/reverse_bfs_children_collector_test.cpp
        memory_mapper/naive_memory_mapper_test.cpp
        meta_parser/yaml_metadata_parser_test.cpp
        app/app_test.cpp
        )

file(COPY ../resources/ DESTINATION ${qc_tests_BINARY_DIR}/src/resources/)

target_link_libraries(qc_tests gtest gtest_main gmock gmock_main)
target_link_libraries(qc_tests qc_lib)