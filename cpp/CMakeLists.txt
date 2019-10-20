cmake_minimum_required(VERSION 3.7)
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

FILE(GLOB HEADERS
    "include/*.h"
    "src/*.h"
    "src/impl/unix/*.h"
)

ENDIF()

add_subdirectory(samples)

add_library(ydlidar_driver SHARED ${SDK_SRC})
IF (WIN32)
ELSE()
target_link_libraries(ydlidar_driver rt pthread)
# target_link_libraries(ydlidar_driver rt ${CMAKE_THREAD_LIBS_INIT})
ENDIF()

# install(TARGETS ydlidar_driver DESTINATION lib)
# install(FILES ${HEADERS} DESTINATION include/ydlidar)

# # Package Generator ------------------
# if(APPLE)
#     set(CPACK_GENERATOR "DMG")
#     # PackageMaker (Mac OSX) packaging.
#     # find_program(PACKAGE_MAKER "PackageMaker")
#     # if(PACKAGE_MAKER)
#     #     set(CPACK_GENERATOR "PackageMaker")
#     # endif()
# else()
#     set(CPACK_GENERATOR "DEB")
#     # DEB (Debian, Ubuntu) packaging.
#     find_program(DPKG "dpkg")
#     if(DPKG)
#         set(CPACK_GENERATOR "DEB")
#         if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686")
#             set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
#         elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
#             set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
#         endif()
#         set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Me <support@example.com>")
#         set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
#     endif()
#     # set(CPACK_SOURCE_GENERATOR "TGZ")
# endif()
# SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
# set(CPACK_PACKAGE_VERSION_MAJOR "0")
# set(CPACK_PACKAGE_VERSION_MINOR "2")
# set(CPACK_PACKAGE_VERSION_PATCH "0")
# set(CPACK_DEBIAN_PACKAGE_DEPENDS "msgpack,zlib,gtest")
# set(CPACK_PACKAGE_DESCRIPTION "test")
# SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "test")
# set(CPACK_PACKAGE_CONTACT "Kevin Walchko")
# set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/Debian/postinst")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.html")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/readme.md")
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/README.html")

# set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

# include(CPack)
