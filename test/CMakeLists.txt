cmake_minimum_required(VERSION 2.8)
project(tester)
set(CMAKE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(CxxTest REQUIRED)
cmake_policy(SET CMP0040 NEW)

if(CXXTEST_FOUND)
	  include_directories(${CXXTEST_INCLUDE_DIR})
		enable_testing()
		file(GLOB_RECURSE TESTES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
		set(VAR 0)
		foreach(variable ${TESTES})
			math(EXPR VAR "1 + ${VAR}")
			CXXTEST_ADD_TEST(${PROJECT_NAME}_${VAR} runner_${VAR}.cpp
				                  ${variable})
			target_compile_features(${PROJECT_NAME}_${VAR} PRIVATE cxx_range_for)
			add_custom_command(TARGET ${PROJECT_NAME}_${VAR}
				                 POST_BUILD
												 COMMAND ./${PROJECT_NAME}_${VAR})
		endforeach()
endif()
