#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <sqlite3.h>

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

int callback(void *, int, char **, char **);

enum
{
    LIST_ID,
    LIST_BRAND,
    LIST_PRICE,
    N_COLUMNS
};

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *window;
    GtkBox *Buttons_Main_Box;
    GtkBox *Main_Content;
    
    GtkTreeView *Main_Content_Tree;
    GtkTreeSelection *select;
    GtkTreeViewColumn *content_type_column;
    GtkTreeViewColumn *content_column;
    GtkTreeViewColumn *size_column;
    GtkCellRenderer *content_type_renderer;
    GtkCellRenderer *size_renderer;
    GtkCellRenderer *content_renderer;
    GtkTreeStore *Main_Tree_Store;


    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Main"));
    gtk_builder_connect_signals(builder, NULL);

    Main_Content = GTK_BOX(gtk_builder_get_object(builder, "Main_Content"));
    Main_Content_Tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "Main_Content_Tree"));
    select = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "select"));
    content_type_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_type_column"));
    content_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "content_column"));
    size_column = GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder, "size_column"));
    content_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_renderer"));
    content_type_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "content_type_renderer"));
    size_renderer = GTK_CELL_RENDERER(gtk_builder_get_object(builder, "size_renderer"));
    Main_Tree_Store = GTK_TREE_STORE(gtk_builder_get_object(builder, "Main_Tree_Store"));
    
    gtk_tree_view_column_add_attribute(content_column,content_renderer,"text",0);
    gtk_tree_view_column_add_attribute(content_type_column,content_type_renderer,"text",1);
    gtk_tree_view_column_add_attribute(size_column,size_renderer,"text",2);

    GtkTreeIter iter,iterch;

    gtk_tree_store_append(Main_Tree_Store,&iter,NULL);
    gtk_tree_store_set(Main_Tree_Store,&iter,0,"row 1",-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,1,"row 1 data1",-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,2,"row 1 data2",-1);

    gtk_tree_store_append(Main_Tree_Store,&iterch,&iter);
    gtk_tree_store_set(Main_Tree_Store,&iterch,0,"row 1 child",-1);
    gtk_tree_store_set(Main_Tree_Store,&iterch,1,"row 1 ch data1",-1);
    gtk_tree_store_set(Main_Tree_Store,&iterch,2,"row 1 ch data2",-1);

    gtk_tree_store_append(Main_Tree_Store,&iter,NULL);
    gtk_tree_store_set(Main_Tree_Store,&iter,0,"row 2",-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,1,"row 2 data1",-1);
    gtk_tree_store_set(Main_Tree_Store,&iter,2,"row 2 data2",-1);

    select=gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));


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
int callback(void *model, int argc, char **argv, char **azColName)
{
   GtkTreeIter iter;

   // AFTER PRINTING TO CONSOLE FILL THE MODEL WITH THE DATA

   gtk_list_store_append(GTK_LIST_STORE(model), &iter);
   gtk_list_store_set(GTK_LIST_STORE(model), &iter, LIST_ID, argv[0],
                      LIST_BRAND, argv[1],
                      LIST_PRICE, argv[2],
                      -1);

   return 0;
}