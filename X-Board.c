#include <stdlib.h>
#include <sys/types.h>
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
gchar *selected_value;

void on_destroy();

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


void on_select_changed(GtkWidget *cell)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(cell),&model,&iter)==FALSE)
    {   
        selected_value=NULL;
        printf("Null Val set\n");
        return ;
    }
    gtk_tree_model_get(model,&iter,0,&selected_value,-1);
    char sql[100]={0};
    printf(" on select Val is %s\n",selected_value);
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

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("1.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    gtk_builder_connect_signals(builder, NULL);

    Main_Content_ViewPort = GTK_WIDGET(gtk_builder_get_object(builder, "Main_Content_ViewPort"));
    Main_Tree_Store = GTK_TREE_STORE(gtk_builder_get_object(builder, "Main_Tree_Store"));
    Main_Content_Tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "Main_Content_Tree"));
    content_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_column"));           // col 1
    content_type_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_type_column"));           // col 2
    size_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "size_column"));           // col 1
    content_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_renderer"));              // col 1 renderer
    size_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "size_renderer"));              // col 1 renderer
    content_type_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_type_renderer"));              // col 2 renderer
    selected = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "Main_Tree_Store")); // tree view selection

    File_Details=GTK_TEXT_VIEW(gtk_builder_get_object(builder, "File_Details"));
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (File_Details));
    
    g_object_set( G_OBJECT( content_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( content_type_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( size_renderer ), "xalign", 0.5,NULL ); 

    gtk_tree_view_column_add_attribute(content_column, content_renderer, "text", 0); // attach the renderer to the column
    gtk_tree_view_column_add_attribute(content_type_column, content_type_renderer, "text", 1); // attach the renderer to the column
    gtk_tree_view_column_add_attribute(size_column, size_renderer, "text", 2); // attach the renderer to the column

    
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
    gtk_text_buffer_set_text (buffer, "Select a list item for additional data", -1);


    selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));

    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
