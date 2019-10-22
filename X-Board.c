#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <gdk/gdk.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sqlite3.h>


sqlite3 *DB; 

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *Main_Content_ViewPort;
GtkBox *Buttons_Main_Box;
GtkBox *Main_Content;


GtkTreeStore *Main_Tree_Store;
GtkTreeView *Main_Content_Tree;
GtkTreeViewColumn *content_column;
GtkTreeViewColumn *content_type_column;
GtkTreeViewColumn *size_column;
GtkTreeSelection *selected;
GtkCellRenderer *content_renderer;
GtkCellRenderer *content_type_renderer;
GtkCellRenderer *size_renderer;
GtkTreeIter iter;


GtkTextView *File_Details;
GtkTextBuffer *buffer;

int setdata(void* data, int argc, char** argv, char** azColName) 
{ 

    gtk_tree_store_append(Main_Tree_Store,&iter,NULL);
    gtk_tree_store_set(Main_Tree_Store,&iter,0,&argv[0][0],-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,1,&argv[1][0],-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,2,&argv[2][0],-1);
    return 0; 
} 

static int additional_info(void* data, int argc, char** argv, char** azColName) 
{ 
    if(strcmp(&argv[1][0],"File")!=0)
        gtk_text_buffer_set_text (buffer, "\n\n\n\n\tSelect a file list item for additional data", -1);
    else
    {
        char disp[1000]={0};
        sprintf(disp,"\n\n\n\n\tFILE NAME : %s\n\n\tFILE LOCATION : %s\n\n\
        FILE TYPE : %s\n\n\tDATE ADDED : %s\n\n\tEXISTENCE : %s\0",argv[3],argv[4],argv[5],argv[6],argv[7] );
        gtk_text_buffer_set_text (buffer, disp, -1);
    }
    return 0; 
} 


void on_select_changed(GtkWidget *selection)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;

    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection),&model,&iter)==FALSE)
    {   
        return ;
    }
    gtk_tree_model_get(model,&iter,0,&selected_value,-1);
    char sql[100]={0};
    sprintf(sql,"SELECT * FROM XBOARD_DATA WHERE CONTENT=\"%s\";",selected_value);
    sqlite3_exec(DB, sql, additional_info, 0, NULL);
}

void on_window_destroy(){
    gtk_main_quit();
}

void button_1_clicked(){
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button1\n");
}

void button_2_clicked(){
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button2\n");
}

void button_3_clicked(){
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button3\n");
}



void remove_data(GtkWidget *menu_item_remove, gpointer userdata)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;
    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(userdata),&model,&iter)!=FALSE)
    {   
        char sql[100]={0};
        gtk_tree_model_get(model,&iter,0,&selected_value,-1);
        sprintf(sql,"DELETE FROM XBOARD_DATA WHERE CONTENT=\"%s\";",selected_value);
        sqlite3_exec(DB, sql, NULL, NULL, NULL);
        sqlite3_exec(DB, "COMMIT", NULL, NULL, NULL);
        gtk_tree_store_remove(Main_Tree_Store,&iter);
        gtk_text_buffer_set_text (buffer, "\n\n\n\n\tSelect a file list item for additional data", -1);
    }
}

void view_popup_menu( GdkEventButton *click_event, GtkTreeSelection *selection, gpointer userdata)
{
    GtkWidget *Context_menu, *menu_item_remove;

    Context_menu = gtk_menu_new();

    menu_item_remove = gtk_menu_item_new_with_label("Remove");

    g_signal_connect(menu_item_remove, "activate",
                     (GCallback)remove_data,selection);

    gtk_menu_shell_append(GTK_MENU_SHELL(Context_menu), menu_item_remove);

    gtk_widget_show_all(Context_menu);

    gtk_menu_popup(GTK_MENU(Context_menu), NULL, NULL, NULL, NULL,
                   (click_event != NULL) ? click_event->button : 0,
                   gdk_event_get_time((GdkEvent *)click_event));
}

gboolean view_onButtonPressed( GdkEventButton *click_event, gpointer userdata)
{
    if (click_event->type == GDK_BUTTON_PRESS && click_event->button == 3)
    {
        GtkTreeSelection *selection;

        selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));

        if (gtk_tree_selection_count_selected_rows(selection) <= 1)
        {
            GtkTreePath *path;

            if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(Main_Content_Tree),click_event->x, click_event->y,&path, NULL, NULL, NULL))
            {
                gtk_tree_selection_unselect_all(selection);
                gtk_tree_selection_select_path(selection, path);
                gtk_tree_path_free(path);
                view_popup_menu(click_event, selection, userdata);

            }
        }


        return TRUE; 
    }
    return FALSE;
}

/*
gboolean view_onPopupMenu(GtkWidget *Main_Content_Tree, gpointer userdata)
{
    view_popup_menu(Main_Content_Tree, NULL, userdata);

    return TRUE; 
}
*/

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

    char sql[10000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int flg;
    long size;
    char *abs_path;
    const gchar* new_clipboard_text = "Clipboard test 1";
    if(text == 0 || g_utf8_collate(text, new_clipboard_text) == 0)
    {
        new_clipboard_text = "Clipboard test 2";
    }
    gtk_clipboard_set_text(clipboard, new_clipboard_text, -1);
    //printf("Clipboard text was %s, value is %8X\n",text, *(int*)data);
    flg=abolute_path(text,abs_path);
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
                        \"%s\",\"text\",\"%ld",NULL,NULL,NULL,%d/%d/%d,\"text/plain\",0",
                        text,strlen(text),tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday);
    }

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError); 
    if (exit != SQLITE_OK) { 
        printf("Error Insert\n"); 
        sqlite3_free(messaggeError); 
    } 
    else
        printf("Records created Successfully!\n");

    sqlite3_close(DB); 

}

void add_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;
    gtk_clipboard_request_text(clipboard, add_content, &value);
}



int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("1.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    gtk_builder_connect_signals(builder, NULL);

    Main_Content_ViewPort = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Content_ViewPort"));
    Main_Tree_Store = GTK_TREE_STORE(gtk_builder_get_object(builder, "Main_Tree_Store"));
    Main_Content_Tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "Main_Content_Tree"));
    content_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_column"));   
    content_type_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_type_column"));    
    size_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "size_column"));         
    content_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_renderer"));      
    size_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "size_renderer"));         
    content_type_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_type_renderer"));   
    selected = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "Main_Tree_selector")); 

    File_Details=GTK_TEXT_VIEW(gtk_builder_get_object(builder, "File_Details"));
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (File_Details));
    
    g_object_set( G_OBJECT( content_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( content_type_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( size_renderer ), "xalign", 0.5,NULL ); 

    gtk_tree_view_column_add_attribute(content_column, content_renderer, "text", 0);
    gtk_tree_view_column_add_attribute(content_type_column, content_type_renderer, "text", 1);
    gtk_tree_view_column_add_attribute(size_column, size_renderer, "text", 2);

    g_signal_connect(Main_Content_Tree, "button-press-event", (GCallback)view_onButtonPressed, NULL);
    //g_signal_connect(Main_Content_Tree, "popup-menu", (GCallback)view_onPopupMenu, NULL);

    char *sql= "CREATE TABLE IF NOT EXISTS XBOARD_DATA(\
                    CONTENT 		TEXT NOT NULL	,\
					CONTENT_TYPE	TEXT NOT NULL	,\
					SIZE	 		TEXT NOT NULL	,\
					FILE_NAME      	TEXT	,\
					FILE_LOCATION  	TEXT    ,\
					FILE_TYPE		TEXT	,\
					DATE_ADDED		DATE    ,\
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

    char query[100] = "SELECT CONTENT,CONTENT_TYPE,SIZE FROM XBOARD_DATA;"; 
    sqlite3_exec(DB, query, setdata, NULL, NULL);
    gtk_text_buffer_set_text (buffer, "\n\n\n\n\tSelect a file list item for additional data", -1);


    selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));

    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
