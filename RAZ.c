#include <stdio.h> 
#include <sqlite3.h> 
  
int main(int argc, char** argv) 
{ 
    sqlite3* DB; 
    int exit = 0; 
    exit = sqlite3_open("X-Board.db", &DB); 
  
    if (exit) { 
        printf("Error open DB %s\n",sqlite3_errmsg(DB)); 
        return (-1); 
    } 
    else
        printf("Opened Database Successfully!\n"); 
    sqlite3_close(DB); 
    return (0); 
} 