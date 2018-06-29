include_directories(${PROJECT_SOURCE_DIR}/ARX/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX)
include_directories(${PROJECT_SOURCE_DIR}/ARX/AR/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX/AR2/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX/ARUtil/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX/ARVideo/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX/ARG/include)
include_directories(${PROJECT_SOURCE_DIR}/ARX/KPM/include)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR/include)

message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

add_definitions("-std=c++11")

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../ARX ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARUtil ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR2 ${CMAKE_CURRENT_BINARY_DIR}/../ARX/KPM ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARVideo ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARG)
link_libraries(libARX libAR libAR2 libARVideo libKPM libARG libARUtil)

add_executable(artoolkitx ${PROJECT_SOURCE_DIR}/artoolkitx.js/ARX_js.cpp)

#To generate source maps the -g4 needs to be passed to the LINK_FLAGS
set_target_properties(artoolkitx PROPERTIES LINK_FLAGS "-s MODULARIZE_INSTANCE=1 -s EXPORT_NAME='arxjs' -s EXPORT_ES6=1 -s DEMANGLE_SUPPORT=1 -g4 --bind -s WASM=0 --post-js ${PROJECT_SOURCE_DIR}/artoolkitx.js/ARX_additions.js -s EXPORTED_FUNCTIONS='['_arwGetProjectionMatrix', '_arwQueryTrackableVisibilityAndTransformation', '_arwGetTrackablePatternConfig', '_arwGetTrackablePatternImage', '_arwLoadOpticalParams', '_detectMarker']' -s EXTRA_EXPORTED_RUNTIME_METHODS='['ccall', 'cwrap']'")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artoolkitx.js
    DESTINATION ${PROJECT_SOURCE_DIR}/../SDK/lib
)