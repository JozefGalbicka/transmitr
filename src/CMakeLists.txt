
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
        structures/tree/min_heap.h
        structures/tree/min_heap.c
        structures/tree/min_heap_node.h
        structures/tree/min_heap_node.c
        compression_algorithms/huffman/huffman_tree.h
        compression_algorithms/huffman/frequency_table.h
        compression_algorithms/huffman/huffman_encode.h
        compression_algorithms/huffman/huffman_decode.h
        compression_algorithms/huffman/huffman_core.h
        compression_algorithms/huffman/frequency_table.c
)

add_executable(transmitr
    main.c
)

target_link_libraries(transmitr transmitr_lib)
target_link_libraries(transmitr readline)
