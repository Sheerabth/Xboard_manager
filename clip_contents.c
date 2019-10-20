

#include <gdk/gdk.h>
#include <gtk/gtk.h>


void text_request_callback(GtkClipboard *clipboard,const gchar *text,gpointer data)
{
    const gchar* new_clipboard_text = "Clipboard test 1";
    if(text == 0 || g_utf8_collate(text, new_clipboard_text) == 0)
    {
        new_clipboard_text = "Clipboard test 2";
    }
    gtk_clipboard_set_text(clipboard, new_clipboard_text, -1);
    printf("Clipboard text was %s, value is %8X\n",text, *(int*)data);
    gtk_main_quit();
}

int main(int argc, char** argv)
{
    gtk_init(&argc, &argv);
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    int value = 0xDECAFBAD;
    gtk_clipboard_request_text(clipboard, text_request_callback, &value);
    gtk_main();
    return 0;
}