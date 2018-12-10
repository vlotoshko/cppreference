cmake_minimum_required(VERSION 2.8.12)

project(cppreference)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(LINKER_FLAGS "")
if(LINUX)
	set(LINKER_FLAGS ${LINKER_FLAGS}"-pthread")
endif()

set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

set(CPP_FILES
	"main.cpp"
	"initialization.cpp"
	"nvi_and_crtp.cpp"
	"other.cpp"
	"smart_pointers.cpp"
	"strings.cpp"
	"type_deduction.cpp"
	"move_semantics.cpp"
	"multithreading.cpp"
	"atomic.cpp"
	"lambda.cpp"
	"copy_elision.cpp"
	)

set(HPP_FILES
	"atomic.hpp"
	"copy_elision.h"
	"initialization.hpp"
	"lambda.hpp"
	"move_semantics.hpp"
	"multithreading.hpp"
	"nvi_and_crtp.hpp"
	"other.hpp"
	"smart_pointers.hpp"
	"strings.hpp"
	"type_deduction.hpp"
	)

set(FILES ${CPP_FILES} ${HPP_FILES})

add_executable(${PROJECT_NAME} ${FILES})
