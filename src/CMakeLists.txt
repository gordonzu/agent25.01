set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})


#[[if (EXISTS ${SOURCE_FILES})
    # The library contains header and source files.
    add_library(${CMAKE_PROJECT_NAME}_lib STATIC
        ${SOURCE_FILES}
        ${HEADER_FILES}
    )
else()
    # The library only contains header files.
    add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE)
endif()]]
