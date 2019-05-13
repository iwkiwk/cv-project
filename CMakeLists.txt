cmake_minimum_required(VERSION 3.5)

project(computer_vision_projects)

include_directories(
	${PROJECT_SOURCE_DIR}/include/
	${PROJECT_SOURCE_DIR}/include/armadillo
	)

add_subdirectory(${PROJECT_SOURCE_DIR}/01_basic_operations/)

add_subdirectory(${PROJECT_SOURCE_DIR}/02_median_blur/)

add_subdirectory(${PROJECT_SOURCE_DIR}/03_regression/)
