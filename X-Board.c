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
#include <sys/mman.h>
#include <sqlite3.h>
#include <sys/stat.h>

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
GtkTreeIter iter1;
GtkTreeIter iter;

GtkTextView *File_Details;
GtkTextBuffer *buffer;
GtkClipboard *selected_clipboard;

int absolute_path(char *, char *);

int setdata(void *data, int count, char **cell_data, char **column_name)
{
    gtk_tree_store_append(Main_Tree_Store, &iter1, NULL);
    gtk_tree_store_set(Main_Tree_Store, &iter1, 0, &cell_data[0][0], -1);
    gtk_tree_store_set(Main_Tree_Store, &iter1, 1, &cell_data[1][0], -1);
    gtk_tree_store_set(Main_Tree_Store, &iter1, 2, &cell_data[2][0], -1);
    return 0;
}

static int additional_info(void *data, int argc, char **argv, char **azColName)
{
    if (strcmp(&argv[0][0], "Text") == 0)
    {
        char disp[1000] = {0};
        sprintf(disp, "\n\n\n\n\tDATE ADDED : %s\n\n\tTARGET : %s\0", &argv[3][0], &argv[4][0]);
        gtk_text_buffer_set_text(buffer, disp, -1);
    }
    else if (strcmp(&argv[0][0], "File") == 0)
    {
        char disp[1000] = {0};
        int flg = absolute_path(&argv[2][0], NULL);
        sprintf(disp, "\n\n\n\n\tFILE NAME : %s\n\n\tFILE LOCATION : %s\n\n\
        DATE ADDED : %s\n\n\tTARGET : %s\n\n\tEXISTENCE : %d\0",
                &argv[1][0], &argv[2][0], &argv[3][0], &argv[4][0], flg);
        gtk_text_buffer_set_text(buffer, disp, -1);
    }
    return 0;
}

void on_select_changed(GtkWidget *selected)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selected), &model, &iter) == FALSE)
    {
        return;
    }
    gtk_tree_model_get(model, &iter, 0, &selected_value, -1);
    char sql[500] = {0};
    sprintf(sql, "SELECT CONTENT_TYPE,FILE_NAME,FILE_LOCATION,DATE_ADDED,TARGET FROM XBOARD_DATA WHERE CONTENT=\"%s\";", selected_value);
    sqlite3_exec(DB, sql, additional_info, 0, NULL);
}

void on_window_destroy()
{
    gtk_main_quit();
}

void remove_data(GtkWidget *menu_item_remove, gpointer userdata)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(userdata), &model, &iter) != FALSE)
    {
        char *message, sql[30000] = {0};
        gtk_tree_model_get(model, &iter, 0, &selected_value, -1);
        sprintf(sql, "DELETE FROM XBOARD_DATA WHERE CONTENT=\"%s\";", selected_value);
        int exit = sqlite3_exec(DB, sql, NULL, 0, &message);
        if (exit != SQLITE_OK)
        {
            printf("Error Insert\n");
            sqlite3_free(&message);
        }
        else
            printf("Records created Successfully!\n");
        sqlite3_exec(DB, "COMMIT", NULL, 0, NULL);
        gtk_tree_store_remove(Main_Tree_Store, &iter);
        gtk_text_buffer_set_text(buffer, "\n\n\n\n\tSelect a list item for additional data", -1);
    }
}

void clip_new_content(GtkWidget *menu_add_clip, gpointer userdata)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;
    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(userdata), &model, &iter) != FALSE)
    {
        gtk_tree_model_get(model, &iter, 0, &selected_value, -1);
        gtk_clipboard_set_text(selected_clipboard, selected_value, -1);
    }
}

void view_Context_menu(GdkEventButton *click_event, gpointer userdata)
{
    GtkWidget *Context_menu, *menu_item_remove, *menu_add_clip;
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *selected_value;
    Context_menu = gtk_menu_new();

    menu_item_remove = gtk_menu_item_new_with_label("Remove");
    menu_add_clip = gtk_menu_item_new_with_label("Add to Clipboard");
    g_signal_connect(menu_item_remove, "activate",
                     (GCallback)remove_data, userdata);
    g_signal_connect(menu_add_clip, "activate",
                     (GCallback)clip_new_content, userdata);
    gtk_menu_shell_append(GTK_MENU_SHELL(Context_menu), menu_item_remove);

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(userdata), &model, &iter) != FALSE)
    {
        gtk_tree_model_get(model, &iter, 1, &selected_value, -1);
        if (strcmp(selected_value, "Text") == 0)
            gtk_menu_shell_append(GTK_MENU_SHELL(Context_menu), menu_add_clip);
    }
    gtk_widget_show_all(Context_menu);

    gtk_menu_popup(GTK_MENU(Context_menu), NULL, NULL, NULL, NULL,
                   (click_event != NULL) ? click_event->button : 0,
                   gdk_event_get_time((GdkEvent *)click_event));
}

void mouse_clicked(GtkWidget *treeview, GdkEventButton *click_event, gpointer userdata)
{
    if (click_event->type == GDK_BUTTON_PRESS && click_event->button == 3)
    {

        GtkTreeSelection *selected;

        selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

        if (gtk_tree_selection_count_selected_rows(selected) <= 1)
        {
            GtkTreePath *path;

            if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview),
                                              click_event->x, click_event->y,
                                              &path, NULL, NULL, NULL))
            {
                gtk_tree_selection_unselect_all(selected);
                gtk_tree_selection_select_path(selected, path);
                gtk_tree_path_free(path);
                GtkTreeIter iter;
                GtkTreeModel *model;
                gchar *selected_value;

                if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selected), &model, &iter))
                {
                    view_Context_menu(click_event, selected);
                }
            }
        }
    }
}

int absolute_path(char *file_path, char *full_path)
{
    if (realpath(file_path, full_path) != NULL)
        return 0;
    else
        return -1;
}

void findSize(char *path, char *size)
{
    char str[100];
    sprintf(str, "du -sh --apparent-size %s > size.txt", path);
    system(str);
    FILE *fp;
    fp = fopen("size.txt", "r");
    fscanf(fp, "%[^\t]s", size);
    printf("\nsize=%s\n", size);
    fclose(fp);
}

void add_content(GtkClipboard *clipboard, const gchar *text, gpointer data)
{

    char sql[3000] = {0};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int flg;
    char size[20];

    flg = absolute_path((char *)text, NULL);

    if (!flg)
    {
        findSize((char *)text, size);
        sprintf(sql, "INSERT INTO XBOARD_DATA (CONTENT,CONTENT_TYPE,SIZE,FILE_NAME,FILE_LOCATION,DATE_ADDED,TARGET)\
        VALUES(\"%s\",\"File\",\"%s\",\"%s\",\"%s\",\"%d/%d/%d\",\"x-special/gnome-copied-files\");",
                text, size, rindex(text, '/') + 1, text, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        gtk_tree_store_append(Main_Tree_Store, &iter1, NULL);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 0, text, -1);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 1, "File", -1);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 2, size, -1);
    }
    else
    {
        sprintf(sql, "INSERT INTO XBOARD_DATA (CONTENT,CONTENT_TYPE,SIZE,DATE_ADDED,TARGET) VALUES(\"%s\",\"Text\",\"%d characters\",\"%d/%d/%d\",\"text-plain/\");",
                text, (int)strlen(text), tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        char char_count[15] = {0};
        sprintf(char_count, "%d characters", (int)strlen(text));
        gtk_tree_store_append(Main_Tree_Store, &iter1, NULL);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 0, text, -1);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 1, "Text", -1);
        gtk_tree_store_set(Main_Tree_Store, &iter1, 2, char_count, -1);
    }
    printf("%s\n", sql);
    char *message;
    int exit = sqlite3_exec(DB, sql, NULL, 0, &message);
    if (exit != SQLITE_OK)
    {
        printf("Error Insert\n");
        sqlite3_free(&message);
    }
    else
        printf("Records created Successfully!\n");
}

void clipboard_changed(GtkClipboard *clipboard, GdkEvent *event, gpointer data)
{
    char *text = gtk_clipboard_wait_for_text(clipboard);
    if (text)
    {
        printf("%s\n", text);
        add_content(clipboard, text, data);
    }
}


void clear_list_button_clicked()
{

    int result = 0;
    char *messaggeError, *drop = "DROP TABLE IF EXISTS XBOARD_DATA;";
    sqlite3_exec(DB, drop, NULL, 0, NULL);
    sqlite3_close(DB);

    char *sql = "CREATE TABLE IF NOT EXISTS XBOARD_DATA(\
                    CONTENT 		TEXT NOT NULL	,\
					CONTENT_TYPE	TEXT NOT NULL	,\
					SIZE	 		TEXT NOT NULL	,\
					FILE_NAME      	TEXT	,\
					FILE_LOCATION  	TEXT    ,\
					DATE_ADDED		TEXT    ,\
					TARGET			TEXT	);";

    result = sqlite3_open("X-Board.db", &DB);
    result = sqlite3_exec(DB, sql, NULL, 0, &messaggeError);

    if (result != SQLITE_OK)
    {
        printf("Error Create Table\n");
        sqlite3_free(messaggeError);
    }
    gtk_tree_store_clear(Main_Tree_Store);
    gtk_text_buffer_set_text(buffer, "\n\n\n\n\tSelect a file list item for additional data", -1);
}

void open_DBbrowser()
{
    system("sqlitebrowser X-Board.db");
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
    selected_clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    File_Details = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "File_Details"));
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(File_Details));

    g_object_set(G_OBJECT(content_renderer), "xalign", 0.5, NULL);
    g_object_set(G_OBJECT(content_type_renderer), "xalign", 0.5, NULL);
    g_object_set(G_OBJECT(size_renderer), "xalign", 0.5, NULL);

    gtk_tree_view_column_add_attribute(content_column, content_renderer, "text", 0);
    gtk_tree_view_column_add_attribute(content_type_column, content_type_renderer, "text", 1);
    gtk_tree_view_column_add_attribute(size_column, size_renderer, "text", 2);

    g_signal_connect(Main_Content_Tree, "button-press-event", (GCallback)mouse_clicked, NULL);
    g_signal_connect(selected_clipboard, "owner-change", G_CALLBACK(clipboard_changed), NULL);

    char *sql = "CREATE TABLE IF NOT EXISTS XBOARD_DATA(\
                    CONTENT 		TEXT NOT NULL	,\
					CONTENT_TYPE	TEXT NOT NULL	,\
					SIZE	 		TEXT NOT NULL	,\
					FILE_NAME      	TEXT	,\
					FILE_LOCATION  	TEXT    ,\
					DATE_ADDED		DATE    ,\
					TARGET			TEXT	);";
    int result = 0;
    result = sqlite3_open("X-Board.db", &DB);
    char *messaggeError;
    result = sqlite3_exec(DB, sql, NULL, 0, &messaggeError);

    if (result != SQLITE_OK)
    {
        printf("Error Create Table\n");
        sqlite3_free(messaggeError);
    }

    char query[100] = "SELECT CONTENT,CONTENT_TYPE,SIZE FROM XBOARD_DATA;";
    sqlite3_exec(DB, query, setdata, NULL, NULL);
    gtk_text_buffer_set_text(buffer, "\n\n\n\n\tSelect a file list item for additional data", -1);

    selected = gtk_tree_view_get_selection(GTK_TREE_VIEW(Main_Content_Tree));

    gtk_widget_show_all(window);
    gtk_main();
    return EXIT_SUCCESS;
}
