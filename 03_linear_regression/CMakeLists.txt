cmake_minimum_required(VERSION 3.5)

project(03_linear_regression)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/../bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin/)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/include/
	C:\\opencv\\opencv\\build\\include)

target_link_libraries(${PROJECT_NAME} debug     C:\\opencv\\opencv\\build\\x64\\vc15\\lib\\opencv_world410d.lib)
target_link_libraries(${PROJECT_NAME} optimized C:\\opencv\\opencv\\build\\x64\\vc15\\lib\\opencv_world410.lib)
target_link_libraries(${PROJECT_NAME} debug     ${PROJECT_SOURCE_DIR}/../lib/blas_win64_MT.lib)
target_link_libraries(${PROJECT_NAME} optimized ${PROJECT_SOURCE_DIR}/../lib/blas_win64_MT.lib)
target_link_libraries(${PROJECT_NAME} debug     ${PROJECT_SOURCE_DIR}/../lib/lapack_win64_MT.lib)
target_link_libraries(${PROJECT_NAME} optimized ${PROJECT_SOURCE_DIR}/../lib/lapack_win64_MT.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../bin")
