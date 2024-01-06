
add_library (transmitr_lib
    core/client.h
    core/client.c
    core/server.h
    core/server.c
    structures/list/array_list.h
    structures/list/array_list.c
    ui/console.h
    ui/console.c
    utils/strings.h
    utils/strings.c
    utils/prints.h
    utils/prints.c
    utils/macros.h
    structures/tree/min_heap.h
    structures/tree/min_heap.c
    structures/tree/min_heap_node.h
    structures/tree/min_heap_node.c
    compression_algorithms/huffman/huffman_tree.h
    compression_algorithms/huffman/frequency_table.h
    compression_algorithms/huffman/huffman_core.h
    compression_algorithms/huffman/frequency_table.c
    compression_algorithms/huffman/huffman_tree.c
    compression_algorithms/huffman/code_table.h
    compression_algorithms/huffman/code_table.c
    compression_algorithms/huffman/code_table_node.h
    compression_algorithms/huffman/code_table_node.c
    compression_algorithms/huffman/huffman_core.c
    core/header.c
    core/header.h
        structures/tree/red_black_tree.h
        structures/tree/red_black_tree_node.h
        structures/tree/red_black_tree.c
        structures/tree/red_black_tree_node.c
        compression_algorithms/LZW/LZW_core.c
        compression_algorithms/LZW/LZW_core.h
)

add_executable(transmitr
    main.c
)

target_link_libraries(transmitr transmitr_lib)
target_link_libraries(transmitr readline)
