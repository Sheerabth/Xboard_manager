#include <gtk/gtk.h>
#include<gdk/gdk.h>

void add_content(GtkClipboard *clipboard,GdkAtom *atoms, gint n_atoms, gpointer data)
{
    FILE *fptr;
    fptr=fopen("anc.txt","w");
    for(int i=0;i<n_atoms;i++)
    {
        printf("%s",*(atoms[i].target);
    
    }
    fclose(fptr);
}

void add_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;
    gtk_clipboard_request_targets(clipboard,add_content,value);

    printf("add\n");
}


void remove_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("remove\n");

}

void edit_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("edit\n");

}

void hotkeys_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("hotkeys\n");

}

void history_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("history\n");

}

void clear_list_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("clear\n");

}

void help_button_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("help\n");

}

void button_1_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("button1\n");

}

void button_2_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("button2\n");

}

void button_3_clicked()
{
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;

        printf("button3\n");

}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Main"));
    gtk_builder_connect_signals(builder, NULL);

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