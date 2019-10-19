

#include <gdk/gdk.h>
#include <gtk/gtk.h>


void text_request_callback(GtkClipboard *clipboard,
                           const gchar *text,
                           gpointer data)
{
  
    const gchar* new_clipboard_text = "Clipboard test 1";
    if(text == 0 || g_utf8_collate(text, new_clipboard_text) == 0)
    {
       
        new_clipboard_text = "Clipboard test 2";
    }


    gtk_clipboard_set_text(clipboard, new_clipboard_text, -1);

   
    printf("Clipboard text was %s, value is %8X\n",
           text, *(int*)data);

    gtk_main_quit();
}

int main(int argc, char** argv)
{
    // Standard boilerplate: initialize the toolkit.
    gtk_init(&argc, &argv);

    // Get a handle to the given clipboard. You can also ask for
    // GDK_SELECTION_PRIMARY (the X "primary selection") or
    // GDK_SELECTION_SECONDARY.
    GtkClipboard* clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    // This is just an arbitrary value to pass through to the
    // callback. You could pass a pointer to a local struct or
    // something similar if that was useful to you.
    int value = 0xDECAFBAD;

    // There are more complex things you can place in the clipboard,
    // but this demonstrates text. The callback will be invoked when
    // the clipboard contents has been received.
    //
    // For a much simpler method of getting the text in the clipboard,
    // see gtk_clipboard_wait_for_text(), which is used in the example
    // program clipboard_watch.
    gtk_clipboard_request_text(clipboard, text_request_callback, &value);

    // We have to run the GTK main loop so that the events required to
    // fetch the clipboard contents can be processed.
    gtk_main();

    return 0;
}