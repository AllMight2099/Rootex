# Not using find_package for OpenAL because the modules won't find it
# find_package(OpenAL)
set(OPENAL_FOUND ON)
set(OPENAL_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/")
set(OPENAL_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/libs/Win64/OpenAL32.lib")
set(ALUT_DLL_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/libs/Win64/alut.dll" CACHE INTERNAL "")
set(ALUT_LIB_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/libs/Win64/alut.lib")

set(ROOTEX_INCLUDES
    ${ROOTEX_INCLUDES}
    ${CMAKE_CURRENT_LIST_DIR}/include/
CACHE INTERNAL "")

add_library(OpenAL SHARED stub.cpp)
add_library(ALUT STATIC stub.cpp)

target_include_directories(OpenAL PUBLIC
    ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(ALUT PUBLIC
    ${ALUT_LIB_LIBRARY}
)

target_link_libraries(OpenAL PUBLIC
    ${OPENAL_LIBRARY}
)
