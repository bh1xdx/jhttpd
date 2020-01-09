project(jhttpd CXX)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


set(JHTTPD_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(JHTTPD_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DJHTTPD_LIB)


if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -fpermissive")
endif(NOT WIN32)

include_directories(${JHTTPD_SRC_DIR}/
                    ${JHTTPD_SRC_DIR}/../include
                    )

aux_source_directory(. JHTTPD_SRCS)


file(GLOB_RECURSE JHTTPD_HEADERS "*.h" "*.hpp" "${JHTTPD_SRC_DIR}/../include/*.h")


# Resource for MSFT Windows
if(WIN32)
    file(GLOB_RECURSE JHTTPD_RC "*.rc")
    set(JHTTPD_LINK_GLAGS /DEF:${JHTTPD_SRC_DIR}/jhttpd.def)

    set( JHTTPD_RC_FLAGS "/l 0x809")
endif(WIN32)


# LINK
#set(JHTTPD_LIBS )
#link_directories()

add_library(${PROJECT_NAME} SHARED ${JHTTPD_HEADERS} ${JHTTPD_SRCS}  ${JHTTPD_RC})

set_target_properties(${PROJECT_NAME} PROPERTIES
                       DEBUG_POSTFIX   -d
                       VERSION "${JHTTPD_VERSION_MAJOR}.${JHTTPD_VERSION_MINOR}.${JHTTPD_VERSION_PATCH}" SOVERSION "${JHTTPD_VERSION_SOVER}")

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${JHTTPD_LINK_GLAGS})
endif(WIN32)

target_link_libraries( ${PROJECT_NAME} ${JHTTPD_LIBS} )


