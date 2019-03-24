cmake_minimum_required(VERSION 3.10)
PROJECT("YDLidar-SDK")
#add_definitions(-std=c++11) # Use C++11
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")

find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
endif()

include_directories(include)
include_directories(src)

IF (WIN32)
FILE(GLOB SDK_SRC
  "src/*.cpp"
  "src/*.c"
  "src/*.h"
  "src/impl/windows/*.cpp"
  "src/impl/windows/*.h"
)

ELSE()
FILE(GLOB SDK_SRC
  "src/*.cpp"
  "src/*.c"
  "src/*.h"
  "src/impl/unix/*.cpp"
  "src/impl/unix/*.h"
)


ENDIF()

add_subdirectory(samples)

add_library(ydlidar_driver SHARED ${SDK_SRC})
IF (WIN32)
ELSE()
target_link_libraries(ydlidar_driver rt pthread)
ENDIF()
