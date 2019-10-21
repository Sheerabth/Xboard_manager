#include<gtk/gtk.h>
#include<gdk/gdk.h>


int main(int argc, char *argv[])
{
    char *str;
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "Window_Main"));
    gtk_builder_connect_signals(builder, NULL);

    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    str=gtk_clipboard_wait_for_text(clipboard);
    printf("%s",str);

    g_object_unref(builder);
    gtk_widget_show(window);                
    gtk_main();

    return 0;
}