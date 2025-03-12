cmake_minimum_required(VERSION 3.20)
project(dectable CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)


include(cmake/CPM.cmake)

# Adding Raylib
CPMAddPackage(
		NAME raylib
		GITHUB_REPOSITORY raysan5/raylib
		GIT_TAG 5.5
		GIT_PROGRESS TRUE
)

CPMAddPackage("gh:nlohmann/json@3.11.3")

# sol2

CPMAddPackage(
		NAME sol2
		GITHUB_REPOSITORY ThePhD/sol2
		GIT_TAG v3.3.0
		GIT_PROGRESS TRUE
)


add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib nlohmann_json sol2)

if(MSVC)
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else()
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/resources
		${CMAKE_CURRENT_BINARY_DIR}/resources)
