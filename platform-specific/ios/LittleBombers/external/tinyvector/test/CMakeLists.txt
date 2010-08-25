function(definetest name)
	set(TEST_SRCS ${name}.cpp)
	set(TEST_HDRS ${name}.hpp)
	
	qt4_wrap_cpp(TEST_MOC_SRCS ${TEST_HDRS})
	add_executable(${name} ${TEST_SRCS} ${TEST_MOC_SRCS})
	target_link_libraries(${name} ${QT_LIBRARIES})
	
	add_test(${name} ${name} ${argn})
endfunction(definetest)

function(definebench name)
	set(BENCH_SRCS ${name}.cpp)
	
	add_executable(${name} ${BENCH_SRCS})
	target_link_libraries(${name} ${QT_LIBRARIES})	
endfunction(definebench)

add_subdirectory(packet)
add_subdirectory(storage)
add_subdirectory(tiny)

add_subdirectory(bench)

add_subdirectory(asm)
