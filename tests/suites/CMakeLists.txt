MACRO(add_module file)
    add_library( ${file} MODULE ${file}.c ../utils.c )
    target_link_libraries( ${file} cunit huff)
    set_target_properties( ${file} PROPERTIES
            PREFIX ""
            LIBRARY_OUTPUT_DIRECTORY "."
    )
ENDMACRO(add_module file)

file(GLOB C_FILES RELATIVE "${CMAKE_SOURCE_DIR}/tests/suites" "${CMAKE_SOURCE_DIR}/tests/suites/*.c")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../includes"
        "${CMAKE_CURRENT_SOURCE_DIR}/../")

FOREACH ( module ${C_FILES} )
    string( REGEX REPLACE ".c$" "" module "${module}" )
    message(STATUS "Found test suite: ${module}")
    add_module(${module})
ENDFOREACH ( module ${MODULES} )
