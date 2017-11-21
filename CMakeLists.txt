cmake_minimum_required(VERSION 2.8.12)

project(cppreference)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOMOC ON)

#find_package(Qt5Core)

set(CPP_FILES
	"main.cpp"
	"initialization.cpp"
	"nvi_and_crtp.cpp"
	"other.cpp"
	"smart_pointers.cpp"
	"strings.cpp"
	"type_deduction.cpp"        
        "move_semantics.cpp"
	)

add_executable(${PROJECT_NAME} ${CPP_FILES})

#target_link_libraries(${PROJECT_NAME} Qt5::Core)
