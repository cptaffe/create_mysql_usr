#include <stdio.h>
#include <stdlib.h>
#include <mysql.h> // mysql

#include "sql.h"

// constant values, no need for parameters
// creates user if does not exist with the correct grant.
const char *user_grant = "grant all on *.* to '%s'@'%s' identified by '%s'";
const char *user_grant_nothing = "grant usage on *.* to '%s'@'%s' identified by '%s'";
const char *user_drop = "drop user '%s'@'%s'";

const char *local_host = "local"; // local hostname
const char *remote_host = "%"; // remote hostname

const char *default_password = "guest"; // because Archer

// query function
static int query(MYSQL *con, char *query) {
	if (mysql_query(con, query)) {
		fprintf(stderr, "%s\n", mysql_error(con));
		return 1;
	}
	return 0;
}

int grant_local(MYSQL *con, char *user) {
	char *ug; asprintf(&ug, user_grant, user, local_host, default_password);
	if (query(con, ug)) { return 1; } free(ug);
	return 0;
}

int grant_remote(MYSQL *con, char *user) {
	char *ug; asprintf(&ug, user_grant, user, remote_host, default_password);
	if (query(con, ug)) { return 1; } free(ug);
	return 0;
}

int grant_all(MYSQL *con, char *user) {
	if (grant_local(con, user)) { return 1; }
	if (grant_remote(con, user)) { return 1; }
	return 0;
}

int rm_local(MYSQL *con, char *user) {
	char *ug; asprintf(&ug, user_grant_nothing, user, local_host, default_password);
	if (query(con, ug)) { return 1; } free(ug);
	char *du; asprintf(&du, user_drop, user, local_host);
	if (query(con, du)) { return 3; } free(du);
	return 0;
}

int rm_remote(MYSQL *con, char *user) {
	char *ug; asprintf(&ug, user_grant_nothing, user, remote_host, default_password);
	if (query(con, ug)) { return 1; } free(ug);
	char *du; asprintf(&du, user_drop, user, remote_host);
	if (query(con, du)) { return 3; } free(du);
	return 0;
}

int rm_all(MYSQL *con, char *user) {
	if (rm_local(con, user)) { return 1; }
	if (rm_remote(con, user)) { return 2; }
	return 0;
}
