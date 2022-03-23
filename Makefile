CC = gcc
CFLAGS = -Wall
OUT = detect
OBJ = main.o graph.o

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ)

clean:
	rm -f $(OBJ) $(OUT)
