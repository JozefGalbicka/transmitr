
add_executable(test_utils_strings
    utils/strings.test.c
)
add_executable(test_structures_array_list
    structures/list/array_list.test.c
)
target_link_libraries(test_utils_strings transmitr_lib)
target_link_libraries(test_structures_array_list transmitr_lib)

add_test (NAME utils-strings COMMAND test_utils_strings)
add_test (NAME structures-array-list COMMAND test_structures_array_list)
