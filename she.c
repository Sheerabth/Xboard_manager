#include <stdio.h> 
#include <sqlite3.h> 

int main() 
{ 
	sqlite3* DB; 
	char *sql= "CREATE TABLE XBOARD_DATA IF NOT EXISTS(\
					CONTENT 		TEXT 	,\
					CONTENTTYPE		TEXT 	,\
					SIZE	 		TEXT 	,\
					FILENAME      	TEXT	,\
					FILELOCATION  	TEXT    ,\
					FILETYPE		TEXT	,\
					DATEADDED		DATE    ,\
					TARGET			TEXT	,\
					FILE_EXISTANCE	BIT		);";
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
