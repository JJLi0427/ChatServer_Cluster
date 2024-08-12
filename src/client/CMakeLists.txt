aux_source_directory(. DIR_SRCS)

add_executable(ChatClient ${DIR_SRCS})

target_link_libraries(ChatClient pthread)