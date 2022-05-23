set(SFML_COMPONENTS system window graphics)
set(SFML_TARGETS sfml-system sfml-window sfml-graphics)

find_package(SFML REQUIRED ${SFML_COMPONENTS} CONFIG)

add_custom_command(
    OUTPUT  fragment.cpp
    COMMAND embedfile fragment.cpp ${CMAKE_CURRENT_LIST_DIR}/fragment.frag
    DEPENDS fragment.frag)

add_executable(gol main.cpp double_buffer.h double_buffer.cpp fragment.cpp)

target_link_libraries(gol ${SFML_TARGETS})
