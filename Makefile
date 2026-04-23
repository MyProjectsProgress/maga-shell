TARGET = my_shell
OBJ = main.c input_parser.c
CC = gcc

all:
	$(CC) -o $(TARGET) $(OBJ)

clean:
	rm -f *.o

fclean:
	rm -f $(TARGET)

re: fclean all