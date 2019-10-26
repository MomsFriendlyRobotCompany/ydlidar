cmake_minimum_required(VERSION 3.10)
PROJECT("ydlidar")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
# cmake_host_system_information(RESULT CORES QUERY NUMBER_OF_LOGICAL_CORES)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")

# # gecko.cmake ----------------------------------------------------
# # set(WS "${PROJECT_SOURCE_DIR}/gecko_workspace.txt")
# # set(GECKO_VERSION "gecko")
# set(GECKO_BASE_PATH "/opt/gecko/modules/${PROJECT_NAME}")
# # IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# # else()
# message(STATUS ">> Setting install base path: ${GECKO_BASE_PATH}")
# set(CMAKE_INSTALL_PREFIX ${GECKO_BASE_PATH})
# # ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# set(GECKO_BIN     "${GECKO_BASE_PATH}/bin")
# set(GECKO_LIB     "${GECKO_BASE_PATH}/lib")
# set(GECKO_CMAKE   "${GECKO_BASE_PATH}/lib/cmake")
# set(GECKO_INCLUDE "${GECKO_BASE_PATH}/include/gecko")


list(APPEND ALL_LIBS "")

# Add gecko -----------------------------------------------------
include_directories(/opt/gecko/include)
link_directories(/opt/gecko/lib)

include_directories(/opt/gecko/gecko/include)
link_directories(/opt/gecko/gecko/lib)

# Load in pkg-config support --------------------------------------
find_package(PkgConfig)

# Get msgpack -----------------------------------------------------
# if(APPLE)
#   pkg_check_modules(MessagePack REQUIRED msgpack)
#   if(MessagePack_FOUND)
#       include_directories(${MessagePack_INCLUDE_DIR})
#       message(STATUS ">> MsgPack Found: ${MessagePack_VERSION}")
#       message(STATUS ">> MsgPack: ${MessagePack_LIBRARIES}  ${MessagePack_INCLUDE_DIR}")
#       list(APPEND ALL_LIBS ${ALL_LIBS} ${MessagePack_LIBRARIES})
#
#       # this is a template library, so no cpp
#       include_directories(msgpack)
#
#   else()
#       message(WARNING "\n<<--- Could not find MsgPack --->>\n")
#   endif()
# elseif(UNIX)
#     find_package(msgpack REQUIRED HINTS /opt/gecko/lib/cmake)
#     if(msgpack_FOUND)
#         include_directories(${msgpack_INCLUDE_DIR})
#         message(STATUS ">> MsgPack Found: ${msgpack_VERSION}")
#         message(STATUS ">> MsgPack: ${msgpack_LIBS}  ${msgpack_INCLUDE_DIR}")
#         list(APPEND ALL_LIBS ${msgpack_LIBRARIES})
#
#         # this is a template library, so no cpp
#         include_directories(msgpack)
#
#     else()
#         message(WARNING "\n<<--- Could not find MsgPack --->>\n")
#     endif()
# endif()

# Protobuf ------------------------------------------------------
find_package(Protobuf 3 REQUIRED)
if ( Protobuf_FOUND )
    # include_directories(${Protobuf_INCLUDE_DIRS})
    # include_directories(${CMAKE_CURRENT_BINARY_DIR})
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
else()
    message(ERROR "Couldn't find Protobuf library")
endif()

# Threads ---------------------------------------------------------
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

# ZeroMQ ----------------------------------------------------------
pkg_check_modules(libzmq REQUIRED libzmq)
if(libzmq_FOUND)
    message(STATUS ">> ZeroMQ found: ${libzmq_VERSION}")
    # add_compile_definitions(ZMQ_BUILD_DRAFT_API)
    # add_compile_definitions(ZMQ_CPP11)
    include_directories(${libzmq_INCLUDE_DIRS})
    # list(APPEND ALL_LIBS ${ALL_LIBS} ${libzmq_LIBRARIES})
    list(APPEND ALL_LIBS ${libzmq_LIBRARIES})

    # ZeroMQ CPP ------------------------------------------------------
    # https://github.com/zeromq/cppzmq
    include_directories(zmq)
else()
    message(WARNING "*** Could not find ZeroMQ ***")
endif()


# Build ydlidar library ----------------------------------------------

FILE(GLOB SDK_SRC
    "../cpp/src/*.cpp"
    "../cpp/src/*.c"
    "../cpp/src/*.h"
    "../cpp/src/impl/unix/*.cpp"
    "../cpp/src/impl/unix/*.h"
)

FILE(GLOB HEADERS
    "../cpp/include/*.h"
    "../cpp/src/*.h"
    "../cpp/src/impl/unix/*.h"
)

add_library(${PROJECT_NAME} SHARED ${SDK_SRC})
target_link_libraries(${PROJECT_NAME} rt pthread)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ../cpp/include
        # ../cpp/src
        ../cpp/include/impl/unix
)

# Build ydlidar node ----------------------------------------------
# include_directories(.)

set(LIDAR_BIN "gecko-ydlidar-node")

add_executable(${LIDAR_BIN} pub.cpp)
target_link_libraries(${LIDAR_BIN}
    PUBLIC
        ${ALL_LIBS}
        ${PROJECT_NAME}
        gecko
        proto
        marko
        ${Protobuf_LIBRARIES}
)
target_include_directories(${LIDAR_BIN}
    PUBLIC
        /opt/gecko/gecko/include/gecko/marko
)

# Install ----------------------------------------------------
# set(WS "${PROJECT_SOURCE_DIR}/gecko_workspace.txt")
# set(GECKO_VERSION "gecko")
set(GECKO_BASE_PATH "/opt/gecko/modules/${PROJECT_NAME}")
# IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# else()
message(STATUS ">> Setting install base path: ${GECKO_BASE_PATH}")
set(CMAKE_INSTALL_PREFIX ${GECKO_BASE_PATH})
# ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(GECKO_BIN     "${GECKO_BASE_PATH}/bin")
set(GECKO_LIB     "${GECKO_BASE_PATH}/lib")
set(GECKO_CMAKE   "${GECKO_BASE_PATH}/lib/cmake")
set(GECKO_INCLUDE "${GECKO_BASE_PATH}")

# message(STATUS ">> targetts: ${TARGETS}")

install(TARGETS ${PROJECT_NAME} ${LIDAR_BIN}
    ARCHIVE DESTINATION ${GECKO_LIB}  # static libraries
    LIBRARY DESTINATION ${GECKO_LIB}  # shared libraries
    RUNTIME DESTINATION ${GECKO_BIN}  # executables
)

install(DIRECTORY ../cpp/include
    DESTINATION ${GECKO_INCLUDE}
    FILES_MATCHING PATTERN "*.h"
    PATTERN ".git" EXCLUDE
)

SET(CMAKE_INSTALL_RPATH "/opt/gecko/gecko/lib" "/opt/gecko/modules/ydlidar/lib")
