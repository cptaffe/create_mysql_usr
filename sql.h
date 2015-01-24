
#ifndef SQL_H_
#define SQL_H_

int rm_all(MYSQL *con, char *user);
int rm_remote(MYSQL *con, char *user);
int rm_local(MYSQL *con, char *user);
int grant_all(MYSQL *con, char *user);
int grant_local(MYSQL *con, char *user);
int grant_remote(MYSQL *con, char *user);

#endif // SQL_H_
