project(TestBot)
cmake_minimum_required(VERSION 2.8)
add_definitions(-DDEBUG)
#aux_source_directory(src SRC_LIST)
#aux_source_directory(src/chat SRC_LIST)
#aux_source_directory(src/timer SRC_LIST)
#aux_source_directory(src/helper SRC_LIST)
#aux_source_directory(src/net SRC_LIST)
aux_source_directory(src/sql SRC_LIST)
file(GLOB_RECURSE SRC_LIST *.h *.cpp)

LINK_LIBRARIES(TgBot boost_system pthread ssl crypto pqxx)

add_executable(${PROJECT_NAME} ${SRC_LIST})
add_subdirectory(test)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
	# -Wundef -Wlong-long -Wpadded
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -Wzero-as-null-pointer-constant -Weffc++ ")
endif()
IF(CMAKE_COMPILER_IS_GNUCXX)
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
