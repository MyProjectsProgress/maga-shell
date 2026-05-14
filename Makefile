executable = mega

source_files =  src/main.c \
				src/views/terminal_view.c \
				src/services/parser_service.c \
				src/builtins/builtins.c \
				src/utils/string_utils.c 

compiler = gcc

all:
	$(compiler) -o $(executable) $(source_files)

clean:
	rm -f *.o

fclean:
	rm -f $(executable)

re: fclean all