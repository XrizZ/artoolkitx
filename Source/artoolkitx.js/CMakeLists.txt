include_directories(${PROJECT_SOURCE_DIR}/ARX/include)

add_definitions("-std=c++11")
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../ARX ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARUtil ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR2 ${CMAKE_CURRENT_BINARY_DIR}/../ARX/KPM ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARVideo)
link_libraries(libARX libAR libAR2 libARUtil libKPM libARVideo)


add_executable(artoolkitx ${PROJECT_SOURCE_DIR}/ARX.js/ARBindEM.cpp)

set_target_properties(artoolkitx PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --bind -g4 -s WASM=0")