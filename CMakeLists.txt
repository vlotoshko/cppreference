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

add_executable(${PROJECT_NAME} ${CPP_FILES})
