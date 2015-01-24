
CFLAGS += -g

# mysql compiler flags
MYSQLCFLAGS = -I/usr/local/mysql/include
MYSQLLNKFLAGS = -L/usr/local/mysql/lib -lmysqlclient -lz

# compiler flags
CFLAGS += $(MYSQLCFLAGS)
LNKFLAGS = $(MYSQLLNKFLAGS)

SRC = main.c sql.c
OBJ = $(SRC:.c=.o)
BIN = cusr

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(LNKFLAGS) -o $(BIN) $(OBJ)

$(OBJ): sql.h

clean:
	$(RM) $(OBJ) $(BIN)
