#include<stdio.h>
#include<sqlite3.h>

static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    printf("%s: ", (const char*)data); 
  
    for (i = 0; i < argc; i++) { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n"); 
    return 0; 
} 

int main(int argc, char** argv) 
{ 
    sqlite3* DB; 
    char* messaggeError; 
    int exit = sqlite3_open("X-Board.db", &DB); 
    char query[100] = "SELECT * FROM XBOARD;"; 
  
    printf("STATE OF TABLE BEFORE INSERT\n"); 
  
    sqlite3_exec(DB, query, callback, NULL, NULL);
    return 0;
} 