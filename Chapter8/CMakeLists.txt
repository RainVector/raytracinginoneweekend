include_directories(${PROJECT_SOURCE_DIR}/Chapter2/)
include_directories(${PROJECT_SOURCE_DIR}/Chapter3/)
include_directories(${PROJECT_SOURCE_DIR}/Chapter6/)

add_executable(Chapter8_metal
	main.cpp
	hitable.h
	hitablelist.h
	material.h
	sphere.h
	utility.h)