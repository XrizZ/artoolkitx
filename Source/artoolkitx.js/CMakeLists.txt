include_directories(${PROJECT_SOURCE_DIR}/ARX/include)

add_definitions("-std=c++11")

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../ARX ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARUtil ${CMAKE_CURRENT_BINARY_DIR}/../ARX/AR2 ${CMAKE_CURRENT_BINARY_DIR}/../ARX/KPM ${CMAKE_CURRENT_BINARY_DIR}/../ARX/ARVideo)
link_libraries(libARX libAR libAR2 libARVideo libKPM libARUtil)

add_executable(artoolkitx ${PROJECT_SOURCE_DIR}/artoolkitx.js/ARX_js.cpp)


#To generate source maps the -g4 needs to be passed to the LINK_FLAGS
set_target_properties(artoolkitx PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 -g4 --bind -s WASM=0 --post-js ${PROJECT_SOURCE_DIR}/artoolkitx.js/append.js -s EXPORTED_FUNCTIONS='['_arwGetProjectionMatrix', '_arwQueryTrackableVisibilityAndTransformation', '_arwGetTrackablePatternConfig']' -s EXTRA_EXPORTED_RUNTIME_METHODS='['ccall', 'cwrap']'")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artoolkitx.js
    DESTINATION ${PROJECT_SOURCE_DIR}/../SDK/lib
)