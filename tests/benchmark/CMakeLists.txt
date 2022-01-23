set(TARGET_BENCHMARK benchmark)

set(SRC_BENCHMARK
	testmain.cpp
	b1_accessor.cpp
)

add_executable(
	${TARGET_BENCHMARK}
	${SRC_BENCHMARK}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_BENCHMARK} Threads::Threads)

