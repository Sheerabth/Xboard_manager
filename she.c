#include <stdio.h> 
#include <sqlite3.h> 

int main(int argc, char** argv) 
{ 
	sqlite3* DB; 
	char *sql= "CREATE TABLE Xboard(\
					FILENAME       TEXT	NOT NULL,\ 
					FILELOCATION   TEXT    NOT NULL,\ 
					FILETYPE		TEXT	NOT NULL,\ 
					FILESIZE		TEXT	NOT NULL,\
					DATEADDED      DATE    NOT NULL );";
	int exit = 0; 
	exit = sqlite3_open("X-Board.db", &DB); 
	char* messaggeError; 
	exit = sqlite3_exec(DB, sql, NULL, 0, &messaggeError); 

	if (exit != SQLITE_OK) { 
		printf("Error Create Table\n"); 
		sqlite3_free(messaggeError); 
	} 
	else
		printf("Table created Successfully\n"); 
	sqlite3_close(DB); 
	return (0); 
} 
