#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <glib.h>

#define IMAGE_DIR "/home/marley/Pictures"

// Function to display system information
static void show_system_info(GtkWidget *widget, gpointer data) {
    char *command = "kitty -e fastfetch"; // Open fastfetch in Kitty terminal
    g_spawn_command_line_async(command, NULL);
}

// Function to handle image selection
static void on_image_selected(GtkComboBox *combo, gpointer data) {
    const gchar *image_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    if (image_name) {
        char *argv[] = { "feh", g_strdup_printf("%s/%s", IMAGE_DIR, image_name), NULL };
        g_spawn_async(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL);
        g_free(argv[1]); // Free the dynamically allocated string
    }
}

// Function to load pictures into the combo box
static void show_pictures(GtkWidget *widget, gpointer data) {
    DIR *dir;
    struct dirent *entry;
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(data);
    
    gtk_combo_box_text_remove_all(combo); // Clear previous entries

    dir = opendir(IMAGE_DIR);
    if (dir == NULL) {
        g_print("Could not open %s directory.\n", IMAGE_DIR);
        return;
    }

    // Read image files from the directory
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".jpeg") ||
            strstr(entry->d_name, ".png") || strstr(entry->d_name, ".gif")) {
            gtk_combo_box_text_append(combo, NULL, entry->d_name);
        }
    }
    closedir(dir); // Close the directory after reading
}

// Function to edit the Hyprland configuration
static void edit_hyprland_conf(GtkWidget *widget, gpointer data) {
    char *file_path = g_strdup_printf("%s/.config/hypr/hyprland.conf", getenv("HOME"));
    char *command = g_strdup_printf("kitty -e vim '%s'", file_path); // Open in Kitty terminal
    if (!g_spawn_command_line_async(command, NULL)) {
        g_print("Failed to open editor for %s\n", file_path);
    }

    g_free(file_path);
    g_free(command);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *main_grid;
    GtkWidget *system_info_button;
    GtkWidget *show_pictures_button;
    GtkWidget *edit_conf_button;
    GtkWidget *image_combo;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Main menu grid
    main_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), main_grid);

    system_info_button = gtk_button_new_with_label("System Information");
    g_signal_connect(system_info_button, "clicked", G_CALLBACK(show_system_info), NULL);
    gtk_grid_attach(GTK_GRID(main_grid), system_info_button, 0, 0, 1, 1);

    image_combo = gtk_combo_box_text_new();
    gtk_grid_attach(GTK_GRID(main_grid), image_combo, 0, 1, 1, 1);
    
    show_pictures_button = gtk_button_new_with_label("Load Pictures");
    g_signal_connect(show_pictures_button, "clicked", G_CALLBACK(show_pictures), image_combo);
    gtk_grid_attach(GTK_GRID(main_grid), show_pictures_button, 1, 1, 1, 1);

    g_signal_connect(image_combo, "changed", G_CALLBACK(on_image_selected), NULL);

    edit_conf_button = gtk_button_new_with_label("Edit Hyprland Config");
    g_signal_connect(edit_conf_button, "clicked", G_CALLBACK(edit_hyprland_conf), NULL);
    gtk_grid_attach(GTK_GRID(main_grid), edit_conf_button, 0, 2, 1, 1);

    // Show all widgets
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

