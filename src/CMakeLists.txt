include_directories(${CMAKECAD_SOURCE_DIR}/src)
link_directories(${CMAKECAD_BINARY_DIR}/src)

add_executable(dpgen main.cpp)
add_library(read_write read_write.cpp)
add_library(allnodes allnodes.cpp)
add_library(findfunction findfunction.cpp)
add_library(searchIOF searchIOF.cpp)
add_library(datapathGen datapathGen.cpp)

target_link_libraries(dpgen read_write allnodes findfunction searchIOF datapathGen)



