file(GLOB_RECURSE ImGui ./**.cpp)
file(GLOB_RECURSE ImGuiH ./**.h)

set(ROOTEX_INCLUDES
    ${ROOTEX_INCLUDES}
    ${CMAKE_CURRENT_LIST_DIR}
CACHE INTERNAL "")

add_library(ImGui STATIC ${ImGui} ${ImGuiH})
set_target_properties(ImGui PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(ImGui PUBLIC
    ./
)
