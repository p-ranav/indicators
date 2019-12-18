
add_executable(block_progress_bar block_progress_bar.cpp)
target_link_libraries(block_progress_bar PRIVATE indica::indica)

add_executable(multi_threaded_bar multi_threaded_bar.cpp)
target_link_libraries(multi_threaded_bar PRIVATE indica::indica)

add_executable(progress_bar_set_progress progress_bar_set_progress.cpp)
target_link_libraries(progress_bar_set_progress PRIVATE indica::indica)

add_executable(progress_bar_tick progress_bar_tick.cpp)
target_link_libraries(progress_bar_tick PRIVATE indica::indica)

add_executable(progress_spinner progress_spinner.cpp)
target_link_libraries(progress_spinner PRIVATE indica::indica)

add_executable(time_meter time_meter.cpp)
target_link_libraries(time_meter PRIVATE indica::indica)

add_executable(multi_progress_bar multi_progress_bar.cpp)
target_link_libraries(multi_progress_bar PRIVATE indica::indica)

add_executable(multi_block_progress_bar multi_block_progress_bar.cpp)
target_link_libraries(multi_block_progress_bar PRIVATE indica::indica)
