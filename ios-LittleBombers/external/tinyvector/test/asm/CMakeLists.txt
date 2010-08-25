add_definitions(-save-temps)

set(BlasGeneric_srcs 
	GenericFloatSwap.cpp
	GenericFloatScal.cpp
	GenericFloatCopy.cpp
	GenericFloatAxpy.cpp
	GenericFloatDot.cpp
	GenericFloatNrm2.cpp
)
set(BlasVectorized_srcs 
	VectorizedFloatSwap.cpp
	VectorizedFloatScal.cpp
	VectorizedFloatCopy.cpp
	VectorizedFloatAxpy.cpp
	VectorizedFloatDot.cpp
	VectorizedFloatNrm2.cpp
)

add_library(
	BlasGeneric STATIC
	${BlasGeneric_srcs}
)
add_library(
	BlasVectorized STATIC
	${BlasVectorized_srcs}
)
