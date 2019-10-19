#include <stdio.h> 
#include <sqlite3.h> 
  
int main(int argc, char** argv) 
{ 
    sqlite3* DB; 
    sqlite3_stmt *res;
    int exit = 0; 
    exit = sqlite3_open("X-Board.db", &DB); 
  
    if (exit) { 
        printf("Error open DB %s\n",sqlite3_errmsg(DB)); 
        return (-1); 
    } 
    else
        printf("Opened Database Successfully!\n"); 
            sqlite3_step(res);
    
    if (rc == SQLITE_ROW) {
        printf("%s\n", sqlite3_column_text(res, 0));
    }
    
    sqlite3_finalize(res);
    sqlite3_close(DB); 
    return (0); 
} 