#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <sys/stat.h>
#include<time.h>

int absolute_path(char *file_path, char *full_path) 
{
    if (realpath(file_path, full_path) != NULL)
        return 0;
    else
        return -1;
} 

long int findSize(const char *file_name)
{
    struct stat st;      
    if(stat(file_name,&st)==0)
        return (st.st_size);
    else
        return -1;
}


void add_content(GtkClipboard *clipboard,const gchar *text,gpointer data)
{
    sqlite3* DB; 
    char* messaggeError; 
    int exit = sqlite3_open("X-Board.db", &DB); 

    char sql[10000]={0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int flg;
    long size;
    char *abs_path;
    
    flg=absolute_path(text,abs_path);
    if(!flg)
    {
        sprintf(sql,"INSERT INTO TABLE XBOARD_DATA VALUES\
                        \"%s\",\"File\",\"%ld\",\"%s\",\"%s\",\"%s\",\
                        %d/%d/%d,\"x-speacial/gnome-copied-files\",1",text,findSize(text),text,abs_path,\
                        index(abs_path,'.')+1,tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);

    }
    else
    {
        sprintf(sql,"INSERT INTO TABLE XBOARD_DATA VALUES\
                        \"%s\",\"text\",\"%ld\",NULL,NULL,NULL,%d/%d/%d,\"text/plain\",0",
                        text,strlen(text),tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);
    }

    exit = sqlite3_exec(DB, sql, NULL, 0, &messaggeError); 
    if (exit != SQLITE_OK) { 
        printf("Error Insert\n"); 
        sqlite3_free(messaggeError); 
    } 
    else
        printf("Records created Successfully!\n");

    sqlite3_close(DB); 


    gtk_main_quit();

}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;
    gtk_clipboard_request_text(clipboard, add_content, &value);
    gtk_main();
    return 0;
}
