CC = gcc
CFLAGS = -g
INSTALL_PATH = /usr/local/bin
PROGRAM_NAME = transmitr
HUFF = src/compression_algorithms/huffman
LZW = src/compression_algorithms/LZW
CORE = src/core
LIST = src/structures/list
TREE = src/structures/tree
UT = src/utils

all: pre-build $(PROGRAM_NAME)

pre-build:
	@echo "Building the program..."

$(PROGRAM_NAME): src/main.c $(HUFF)/code_table.c $(HUFF)/code_table_node.c $(HUFF)/frequency_table.c $(HUFF)/huffman_core.c $(HUFF)/huffman_tree.c $(LZW)/LZW_core.c $(CORE)/client.c $(CORE)/header.c $(CORE)/server.c $(LIST)/array_list.c $(TREE)/min_heap.c $(TREE)/min_heap_node.c $(TREE)/red_black_tree.c $(TREE)/red_black_tree_node.c src/ui/console.c $(UT)/prints.c $(UT)/strings.c 
	$(CC) $(CFLAGS) -I src -o $(PROGRAM_NAME) src/main.c $(HUFF)/code_table.c $(HUFF)/code_table_node.c $(HUFF)/frequency_table.c $(HUFF)/huffman_core.c $(HUFF)/huffman_tree.c $(LZW)/LZW_core.c $(CORE)/client.c $(CORE)/header.c $(CORE)/server.c $(LIST)/array_list.c $(TREE)/min_heap.c $(TREE)/min_heap_node.c $(TREE)/red_black_tree.c $(TREE)/red_black_tree_node.c src/ui/console.c $(UT)/prints.c $(UT)/strings.c -lreadline 
	
	@echo "Builded"
help:
	@echo "Available commands:"
	@echo "  all       - Build the program"
	@echo "  install   - Install the program"
	@echo "  uninstall - Uninstall the program"
	@echo "  clean     - Clean the build"
	@echo "  help      - Show help"

install: $(PROGRAM_NAME)
	@echo "Installing the program..."
	@sudo cp $(PROGRAM_NAME) $(INSTALL_PATH)/$(PROGRAM_NAME)
	@echo "Instaled"
uninstall:
	@echo "Uninstalling the program..."
	@sudo rm -f $(INSTALL_PATH)/$(PROGRAM_NAME)
	@echo "Uninstaled"

clean:
	@echo "Cleaning up..."
	@rm -f $(PROGRAM_NAME)
	@echo "Cleaned up"

