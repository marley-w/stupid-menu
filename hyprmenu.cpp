// file: hyprmenu.cpp
#include <gtk/gtk.h>
#include <iostream>
#include <unistd.h>  // for fork(), execlp()
#include <sys/wait.h>
#include <cstring>  // for strcat()

// Function to launch applications independently using fork and exec
void launch_app(const char *app) {
    pid_t pid = fork();  // Create a new process
    if (pid == 0) {
        // Child process: execute the application
        execlp(app, app, (char *)NULL);
        _exit(EXIT_FAILURE);  // Exit child if exec fails
    } else if (pid > 0) {
        // Parent process: do nothing, just return to GTK main loop
    } else {
        std::cerr << "Fork failed!" << std::endl;
    }
}

// Function to launch terminal-based vim using fork and exec
void edit_config(const char *config_file) {
    pid_t pid = fork();  // Create a new process
    if (pid == 0) {
        // Child process: run kitty or gnome-terminal
        
        // Get the home directory path
        const char *home_dir = getenv("HOME");
        if (home_dir == NULL) {
            std::cerr << "Could not get HOME environment variable" << std::endl;
            _exit(EXIT_FAILURE);
        }

        // Construct the full path to the config file
        std::string full_path = std::string(home_dir) + "/" + config_file;

        // Use kitty or fallback to gnome-terminal
        if (execlp("kitty", "kitty", "-e", "vim", full_path.c_str(), (char *)NULL) == -1) {
            execlp("gnome-terminal", "gnome-terminal", "--", "vim", full_path.c_str(), (char *)NULL);
        }

        _exit(EXIT_FAILURE);  // Exit child if exec fails
    } else if (pid > 0) {
        // Parent process: continue with GTK application (non-blocking)
    } else {
        std::cerr << "Fork failed!" << std::endl;
    }
}

// Function to reboot the system
void reboot_system() {
    std::system("reboot");
}

// Function to shut down the system
void shutdown_system() {
    std::system("shutdown now");
}

// Callback for the buttons to launch apps, edit files, etc.
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *command = static_cast<const char *>(data);
    launch_app(command);
}

// Callback for editing config files
void on_edit_config_clicked(GtkWidget *widget, gpointer data) {
    const char *config_file = static_cast<const char *>(data);
    edit_config(config_file);
}

// Main application
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "HyprMenu");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a notebook widget (for tabs)
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // ==== Applications Tab ====
    GtkWidget *applications_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *steam_button = gtk_button_new_with_label("Launch Steam");
    GtkWidget *firefox_button = gtk_button_new_with_label("Launch Firefox");
    GtkWidget *kitty_button = gtk_button_new_with_label("Launch Kitty");

    // Connect buttons to commands (non-blocking)
    g_signal_connect(steam_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "steam");
    g_signal_connect(firefox_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "firefox");
    g_signal_connect(kitty_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "kitty");

    // Add buttons to the Applications tab
    gtk_box_pack_start(GTK_BOX(applications_box), steam_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), firefox_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), kitty_button, TRUE, TRUE, 0);

    // Add Applications tab to the notebook
    GtkWidget *applications_label = gtk_label_new("Applications");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), applications_box, applications_label);

    // ==== Edit Configs Tab ====
    GtkWidget *configs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *hyprland_button = gtk_button_new_with_label("Edit hyprland.conf");
    GtkWidget *hyprpaper_button = gtk_button_new_with_label("Edit hyprpaper.conf");
    GtkWidget *kitty_button_config = gtk_button_new_with_label("Edit kitty.conf");
    GtkWidget *waybar_style_button = gtk_button_new_with_label("Edit waybar style.css");
    GtkWidget *waybar_config_button = gtk_button_new_with_label("Edit waybar config");

    // Connect buttons to edit config files with vim
    g_signal_connect(hyprland_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/hypr/hyprland.conf");
    g_signal_connect(hyprpaper_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/hypr/hyprpaper.conf");
    g_signal_connect(kitty_button_config, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/kitty/kitty.conf");
    g_signal_connect(waybar_style_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/waybar/style.css");
    g_signal_connect(waybar_config_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/waybar/config");

    // Add config buttons to the Edit Configs tab
    gtk_box_pack_start(GTK_BOX(configs_box), hyprland_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), hyprpaper_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), kitty_button_config, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), waybar_style_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), waybar_config_button, TRUE, TRUE, 0);

    // Add Edit Configs tab to the notebook
    GtkWidget *configs_label = gtk_label_new("Edit Configs");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), configs_box, configs_label);

    // ==== System Information Tab ====
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *system_info_label = gtk_label_new("CPU: i5-7600k @ 4.6GHz\n"
                                                 "GPU: RX580 (8GB)\n"
                                                 "OS: Arch Linux Zen");
    gtk_box_pack_start(GTK_BOX(info_box), system_info_label, TRUE, TRUE, 0);

    // Load and display the logo image at original size
    GtkWidget *logo_image = gtk_image_new_from_file("~/Pictures/logogirl.png");
    if (logo_image != NULL) {
        gtk_box_pack_start(GTK_BOX(info_box), logo_image, TRUE, TRUE, 0);
    }

    // Add System Information tab to the notebook
    GtkWidget *info_label = gtk_label_new("System Information");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), info_box, info_label);

    // ==== Bye Tab ====
    GtkWidget *bye_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *reboot_button = gtk_button_new_with_label("Reboot");
    GtkWidget *shutdown_button = gtk_button_new_with_label("Shutdown");

    // Connect buttons to system commands
    g_signal_connect(reboot_button, "clicked", G_CALLBACK(reboot_system), NULL);
    g_signal_connect(shutdown_button, "clicked", G_CALLBACK(shutdown_system), NULL);

    // Add buttons to the Bye tab
    gtk_box_pack_start(GTK_BOX(bye_box), reboot_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(bye_box), shutdown_button, TRUE, TRUE, 0);

    // Add Bye tab to the notebook
    GtkWidget *bye_label = gtk_label_new("Bye");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), bye_box, bye_label);

    // Show everything
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

