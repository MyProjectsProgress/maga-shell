executable = mega

source_files =  src/main.c \
				src/contorllers/shell_contoller.c \
				src/builtins/cd.c \
				src/builtins/echo.c \
				src/builtins/pwd.c \
				src/builtins/exit.c \
				src/builtins/env.c \
				src/builtins/which.c \
				src/views/terminal_view.c \
				src/services/parser_service.c \
				src/services/executor_service.c \
				src/utils/string_utils.c \
				src/utils/path_utils.c \

compiler = gcc

all:
	$(compiler) -o $(executable) $(source_files)

clean:
	rm -f *.o

fclean:
	rm -f $(executable)

re: fclean all