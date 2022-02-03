set(TARGET_TEST unittest)

set(SRC_TEST
	testmain.cpp
	test_getter.cpp
	test_setter.cpp
	test_accessor_basic.cpp
	test_accessor_default_no_getter_setter.cpp
	test_accessor_operator_binary.cpp
	test_accessor_operator_binary_assignment.cpp
	test_accessor_operator_logic.cpp
	test_accessor_operator_unary.cpp
)

add_executable(
	${TARGET_TEST}
	${SRC_TEST}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TEST} Threads::Threads)

set_target_properties(${TARGET_TEST} PROPERTIES CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
if(coverage)
	set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
endif()
endif()

add_test(NAME ${TARGET_TEST} COMMAND ${TARGET_TEST})
