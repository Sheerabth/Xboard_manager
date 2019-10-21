#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sqlite3.h>

sqlite3* DB; 

GtkBuilder *builder;
GtkWidget *window;
GtkBox *Buttons_Main_Box;
GtkBox *Main_Content;

GtkTreeView *Main_Content_Tree;
GtkTreeSelection *selected;
GtkTreeViewColumn *content_type_column;
GtkTreeViewColumn *content_column;
GtkTreeViewColumn *size_column;
GtkCellRenderer *content_type_renderer;
GtkCellRenderer *size_renderer;
GtkCellRenderer *content_renderer;
GtkTreeStore *Main_Tree_Store;
GtkTreeIter iter;

GtkButton *AddButton;
GtkButton *RemoveButton;
GtkButton *EditButton;
GtkButton *HistoryButton;
GtkButton *ClearButton;
GtkButton *HelpButton;

GtkTextView *File_Details;
GtkTextBuffer *buffer;


static int setdata(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    gtk_tree_store_append(Main_Tree_Store,&iter,NULL);
    gtk_tree_store_set(Main_Tree_Store,&iter,0,argv[0],-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,1,argv[1],-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,2,argv[2],-1);
    return 0; 
} 


void add_content(GtkClipboard *clipboard, GdkAtom *atoms, gint n_atoms, gpointer data)
{
    FILE *fptr;
    fptr = fopen("anc.txt", "w");
    for (int i = 0; i < n_atoms; i++)
    {
    }
    fclose(fptr);
}

void add_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;
    gtk_clipboard_request_targets(clipboard, add_content, value);

    printf("add\n");
}

void remove_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("remove\n");
}

void edit_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("edit\n");
}

void hotkeys_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("hotkeys\n");
}

void history_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("history\n");
}

void clear_list_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("clear\n");
}

void help_button_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("help\n");
}

void button_1_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button1\n");
}

void button_2_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button2\n");
}

void button_3_clicked()
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

    printf("button3\n");
}



int main(int argc, char *argv[])
{
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Main"));
    gtk_builder_connect_signals(builder, NULL);

    AddButton=GTK_BUTTON(gtk_builder_get_object(builder, "Add_Button"));
    RemoveButton=GTK_BUTTON(gtk_builder_get_object(builder, "Remove_Button"));
    EditButton=GTK_BUTTON(gtk_builder_get_object(builder, "Edit_Button"));
    HistoryButton=GTK_BUTTON(gtk_builder_get_object(builder, "History"));
    ClearButton=GTK_BUTTON(gtk_builder_get_object(builder, "Clear_Button"));
    HelpButton=GTK_BUTTON(gtk_builder_get_object(builder, "Help"));

    File_Details=GTK_TEXT_VIEW(gtk_builder_get_object(builder, "File_Details"));
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (File_Details));

    Main_Content = GTK_BOX(gtk_builder_get_object(builder, "Main_Content"));
    Main_Content_Tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "Main_Content_Tree"));
    selected = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "selected"));
    content_type_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_type_column"));
    content_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_column"));
    size_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "size_column"));
    content_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_renderer"));
    content_type_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_type_renderer"));
    size_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "size_renderer"));
    Main_Tree_Store = GTK_TREE_STORE(gtk_builder_get_object(builder, "Main_Tree_Store"));
    
    g_object_set( G_OBJECT( content_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( content_type_renderer ), "xalign", 0.5,NULL );  
    g_object_set( G_OBJECT( size_renderer ), "xalign", 0.5,NULL );  
    gtk_tree_view_column_add_attribute(content_column,content_renderer,"text",0);
    gtk_tree_view_column_add_attribute(content_type_column,content_type_renderer,"text",1);
    gtk_tree_view_column_add_attribute(size_column,size_renderer,"text",2);

   

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

    

    selected=gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));


    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// called when window is closed
void on_Main_Window_destroy()
{
    gtk_main_quit();
}
