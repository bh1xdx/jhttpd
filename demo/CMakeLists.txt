Project(jhttpd_demo CXX)

# SRC.
set(JHTTPD_DEMO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(.
                    ../include
                    )


aux_source_directory(. JHTTPD_DEMO_SRCS)

file(GLOB_RECURSE JHTTPD_DEMO_HEADERS "*.h")

# LINK
set(JHTTPD_DEMO_LIBS
        debug jhttpd-d optimized jhttpd
    )


message(STATUS "JHDMEO-LIBS: ${JHTTPD_DEMO_LIBS}")

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
                 )

message(STATUS "DEMO-LIB-DIRS: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_executable(${PROJECT_NAME} ${JHTTPD_DEMO_HEADERS} ${JHTTPD_DEMO_SRCS})

target_link_libraries( ${PROJECT_NAME} ${JHTTPD_DEMO_LIBS} )

add_dependencies(${PROJECT_NAME} jhttpd)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX   -d)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})


