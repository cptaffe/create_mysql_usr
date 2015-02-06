
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h> // mysql

#include "sql.h"

// configuration
#define MYSQL_HOST "sally.host.ualr.edu"
#define MYSQL_USERNAME "cptaffe"
#define MYSQL_PASSWORD "pass"
#define MYSQL_DB "mysql"

enum op_state {
	GIVE_ALL,
	GIVE_LOC,
	GIVE_RMT,
	TAKE_ALL,
	TAKE_LOC,
	TAKE_RMT
};

static void usage(char *name) {
	fprintf(stderr, "usage: %s (giveth | taketh_away) [local | remote]\n", name);
}

int main(int argc, char *argv[]) {
	// client version information
	printf("MySQL client version: %s\n", mysql_get_client_info());

	// init mysql connection
	MYSQL *con = mysql_init(NULL);
	if (con == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}

	// connect to database
	if (mysql_real_connect(con,
		MYSQL_HOST,
		MYSQL_USERNAME,
		MYSQL_PASSWORD,
		MYSQL_DB,
		0, NULL, 0) == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return 2;
	}

	enum op_state ops;

	// set operation state based on args.
	if (argc > 1) {
		if (!strcmp(argv[1], "giveth")) {
			if (argc == 2) {
				ops = GIVE_ALL;
			} else if (argc == 3) {
				if (!strcmp(argv[2], "local")) {
					ops = GIVE_LOC;
				} else if (!strcmp(argv[2], "remote")) {
					ops = GIVE_RMT;
				} else {
					usage(argv[0]);
					return 6;
				}
			} else {
				usage(argv[0]);
				return 7;
			}
		} else if (!strcmp(argv[1], "taketh_away")) {
			if (argc == 2) {
				ops = TAKE_ALL;
			} else if (argc == 3) {
				if (!strcmp(argv[2], "local")) {
					ops = TAKE_LOC;
				} else if (!strcmp(argv[2], "remote")) {
					ops = TAKE_RMT;
				} else {
					usage(argv[0]);
					return 11;
				}
			} else {
				usage(argv[0]);
				return 12;
			}
		} else {
			usage(argv[0]);
			return 13;
		}
	} else {
		usage(argv[0]);
		return 14;
	}

	// print intentions
	if (ops == GIVE_ALL) {
		puts("giving local and remote accounts to input username(s)");
	} else if (ops == GIVE_LOC) {
		puts("giving local account to input username(s)");
	} else if (ops == GIVE_RMT) {
		puts("giving remote account to input username(s)");
	} else if (ops == TAKE_ALL) {
		puts("taking away local and remote accounts to input username(s)");
	} else if (ops == TAKE_LOC) {
		puts("taking away local account to input username(s)");
	} else if (ops == TAKE_RMT) {
		puts("taking away remote account to input username(s)");
	}

	// SQL injection possible!
	size_t user_name_size = 16; // longest possible username
	char *user_name = malloc(user_name_size);
	for (int i = 0; getline(&user_name, &user_name_size, stdin) != -1; i++) {
		size_t len = strlen(user_name);
		if (len == 0) {continue;} // blank line
		if (len > 17) {
			printf("line %d: username too long.\n", i);
		} else {
			if (user_name[len - 1] == '\n') { user_name[len - 1] = 0; }

			// do operation for mode
			if (ops == GIVE_ALL) {
				if (grant_all(con, user_name)) { return 3; }
			} else if (ops == GIVE_LOC) {
				if (grant_local(con, user_name)) { return 4; }
			} else if (ops == GIVE_RMT) {
				if (grant_remote(con, user_name)) { return 5; }
			} else if (ops == TAKE_ALL) {
				if (rm_all(con, user_name)) { return 8; }
			} else if (ops == TAKE_LOC) {
				if (rm_local(con, user_name)) { return 9; }
			} else if (ops == TAKE_RMT) {
				if (rm_remote(con, user_name)) { return 10; }
			}
		}
	}
	free(user_name);
	mysql_close(con);
}
