#include <gtk/gtk.h>

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
void on_Window_Main_destroy()
{
    gtk_main_quit();
}