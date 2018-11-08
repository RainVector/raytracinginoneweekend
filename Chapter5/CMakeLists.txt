include_directories(${PROJECT_SOURCE_DIR}/Chapter2/)
include_directories(${PROJECT_SOURCE_DIR}/Chapter3/)


add_executable(Chapter5_normalVisual
	normalVisual.cpp)
add_executable(Chapter5_multiobj
	multiobj.cpp
	hitable.h
	hitablelist.h
	sphere.h)