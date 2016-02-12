cmake_minimum_required(VERSION 2.8)
project (Server)
file(GLOB src
"Server/*.cpp"
)
add_definitions(-std=c++11)
add_executable(Server ${src})
#Set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Set include dirs
include_directories("Server")
include_directories("../include/soci")
include_directories("../include/")
include_directories("../include/oolua")
include_directories("../include/lua")
include_directories("/usr/include/mysql")

#Find and link Libraries 
find_package (Soci REQUIRED)
if (SOCI_FOUND)
include_directories(${SOCI_INCLUDE_DIRS})
target_link_libraries(Server "soci_core")
target_link_libraries(Server "soci_mysql")
endif(SOCI_FOUND)

find_package (Lua REQUIRED)
if (LUA_FOUND)
include_directories(${Lua_INCLUDE_DIRS})
target_link_libraries(Server ${LUA_LIBRARIES})
endif (LUA_FOUND)

target_link_libraries(Server "oolua")
target_link_libraries(Server "${CMAKE_SOURCE_DIR}/../lib/linux/libjinra.a")
target_link_libraries(Server "pthread")

#Set binary output and copy config
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")
configure_file("Server/Data/config.ini" "${EXECUTABLE_OUTPUT_PATH}/Data/config.ini" COPYONLY)