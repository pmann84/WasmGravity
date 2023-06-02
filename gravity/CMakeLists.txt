file(GLOB_RECURSE CORE_HDR *.hpp)
file(GLOB_RECURSE CORE_SRC *.cpp)

add_executable(gravity_lib ${CORE_SRC} ${CORE_HDR})

set_target_properties(gravity_lib PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --bind")

add_custom_command(TARGET gravity_lib POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE_DIR:gravity_lib>/gravity_lib.js"
        "${CMAKE_SOURCE_DIR}/browser/gravity_lib.js"
        COMMENT "Copying JS to output directory")

add_custom_command(TARGET gravity_lib POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
        "$<TARGET_FILE_DIR:gravity_lib>/gravity_lib.wasm"
        "${CMAKE_SOURCE_DIR}/browser/gravity_lib.wasm"
        COMMENT "Copying JS to output directory")