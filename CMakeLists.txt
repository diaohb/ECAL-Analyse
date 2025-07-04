#set(CMAKE_CXX_STANDARD 14)
###############################################
# Project hbuana
###############################################

#cmake minimum requirement
cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Project name and version
project ( compare VERSION 1.0.0 LANGUAGES C CXX )

#CMake type
set(CMAKE_BUILD_TYPE Debug)

#External packages
find_package( ROOT COMPONENTS Matrix Hist RIO MathCore Physics Minuit ROOTDataFrame)
# find_package( yaml-cpp REQUIRED)

#set run time output directory as bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#Add include directories
include_directories(src)
include_directories(include)
# include_directories(${YAML_CPP_INCLUDE_DIR})
#message("hey ${YAML_CPP_INCLUDE_DIR}")
# message(${ROOT_USE_FILE})
include(${ROOT_USE_FILE})
# message(STATUS "ROOT_USE_FILE: ${ROOT_USE_FILE}")


#Add static librarys
# add_library(HBase STATIC src/HBase.cxx)

#add executable
add_executable(disp src/disp.cxx src/TreeReader.cxx)
add_executable(disp_hough src/disp_hough.cxx src/TreeReader.cxx)
add_executable(dispe src/dispe.cxx src/TreeReader.cxx)
add_executable(hitmap src/hitmap.cxx src/TreeReader.cxx)
add_executable(hough src/hough.cxx src/TreeReader.cxx)
add_executable(mip_layer src/mip_layer.cxx src/TreeReader.cxx)
add_executable(mip_layer_hough src/mip_layer_hough.cxx src/TreeReader.cxx)
add_executable(calib src/reconstruct.cxx src/TreeReader.cxx)
add_executable(analyse src/analyse.cxx src/TreeReader.cxx)
#link libraries
target_link_libraries(disp ${ROOT_LIBRARIES})
target_link_libraries(disp_hough ${ROOT_LIBRARIES})
target_link_libraries(dispe ${ROOT_LIBRARIES})
target_link_libraries(hitmap ${ROOT_LIBRARIES})
target_link_libraries(hough ${ROOT_LIBRARIES})
target_link_libraries(mip_layer ${ROOT_LIBRARIES})
target_link_libraries(mip_layer_hough ${ROOT_LIBRARIES})
target_link_libraries(calib ${ROOT_LIBRARIES})
target_link_libraries(analyse ${ROOT_LIBRARIES})

#Add scripts to make setup.sh to include hbuana into environment
# execute_process(COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/config/setup.sh ${PROJECT_BINARY_DIR})
# execute_process(COMMAND sed -i "s:PROJECTHERE:${CMAKE_CURRENT_SOURCE_DIR}:g" ${PROJECT_BINARY_DIR}/setup.sh)
# execute_process(COMMAND chmod +x ${PROJECT_BINARY_DIR}/setup.sh)
