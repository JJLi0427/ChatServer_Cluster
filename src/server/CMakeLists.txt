aux_source_directory(. MAIN_SRCS)
aux_source_directory(./database DB_SRCS)
aux_source_directory(./model MODEL_SRCS)

add_executable(ChatServer ${MAIN_SRCS} ${DB_SRCS} ${MODEL_SRCS})

target_link_libraries(ChatServer muduo_net muduo_base mysqlclient pthread hiredis)