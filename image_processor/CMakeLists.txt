cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#set(SOURCE_FILES image.cpp)

add_executable(image_processor ${SOURCE_FILES} image_processor.cpp bmp.cpp bmp.h image.cpp image.h filters.cpp filters.h parses.cpp parses.h filter_factory.cpp filter_factory.h)

#set(TEST_FILES test.cpp)

#add_catch(test_image_processor ${SOURCE_FILES} ${TEST_FILES})

