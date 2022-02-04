set(TARGET_TUTORIAL tutorial)

set(SRC_TUTORIAL
	tutorialmain.cpp
	tutorial_accessor.cpp
	tutorial_view_model_binding.cpp
)

add_executable(
	${TARGET_TUTORIAL}
	${SRC_TUTORIAL}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TUTORIAL} Threads::Threads)

set_target_properties(${TARGET_TUTORIAL} PROPERTIES CXX_STANDARD 20)

