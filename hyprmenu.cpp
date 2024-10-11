// file: hyprmenu.cpp
#include <gtk/gtk.h>
#include <iostream>
#include <unistd.h>  // for fork(), execlp()
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>   // for getenv()

// Function to launch applications independently using fork and exec
void launch_app(const char *app) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(app, app, (char *)NULL);  // Try to launch the app
        std::cerr << "Failed to launch: " << app << std::endl;  // Error logging if execlp fails
        _exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process does nothing
    } else {
        std::cerr << "Fork failed!" << std::endl;
    }
}

// Function to open config files in vim or fallback to nano
void edit_config(const char *config_file) {
    pid_t pid = fork();
    if (pid == 0) {
        // Get the home directory
        const char *home_dir = getenv("HOME");
        if (home_dir == NULL) {
            std::cerr << "Could not get HOME environment variable" << std::endl;
            _exit(EXIT_FAILURE);
        }

        // Construct full path to config file
        std::string full_path = std::string(home_dir) + "/" + config_file;

        // Try opening with kitty or fallback to gnome-terminal and nano
        if (execlp("kitty", "kitty", "-e", "vim", full_path.c_str(), (char *)NULL) == -1) {
            if (execlp("gnome-terminal", "gnome-terminal", "--", "vim", full_path.c_str(), (char *)NULL) == -1) {
                std::cerr << "Failed to open config file: " << full_path << std::endl;
                execlp("gnome-terminal", "gnome-terminal", "--", "nano", full_path.c_str(), (char *)NULL);  // Fallback to nano
            }
        }
        _exit(EXIT_FAILURE);
    }
}

// System update function with multiple distro fallbacks
void system_update() {
    pid_t pid = fork();
    if (pid == 0) {
        if (execlp("kitty", "kitty", "-e", "sudo", "pacman", "-Syu", (char *)NULL) == -1) {
            if (execlp("kitty", "kitty", "-e", "sudo", "apt", "update", "&&", "sudo", "apt", "upgrade", "-y", (char *)NULL) == -1) {
                if (execlp("kitty", "kitty", "-e", "sudo", "dnf", "update", "-y", (char *)NULL) == -1) {
                    execlp("kitty", "kitty", "-e", "sudo", "zypper", "update", (char *)NULL);
                }
            }
        }
        _exit(EXIT_FAILURE);
    }
}

// Function to launch terminal apps with fallbacks
void launch_terminal() {
    pid_t pid = fork();
    if (pid == 0) {
        if (execlp("kitty", "kitty", (char *)NULL) == -1) {
            if (execlp("alacritty", "alacritty", (char *)NULL) == -1) {
                if (execlp("gnome-terminal", "gnome-terminal", (char *)NULL) == -1) {
                    execlp("xterm", "xterm", (char *)NULL);
                }
            }
        }
        _exit(EXIT_FAILURE);
    }
}

// Function to launch browser apps with fallbacks
void launch_browser() {
    pid_t pid = fork();
    if (pid == 0) {
        if (execlp("firefox", "firefox", (char *)NULL) == -1) {
            if (execlp("chromium", "chromium", (char *)NULL) == -1) {
                execlp("brave", "brave", (char *)NULL);
            }
        }
        _exit(EXIT_FAILURE);
    }
}

// Callback to launch generic applications
void on_button_clicked(GtkWidget *widget, gpointer data) {
    const char *app = static_cast<const char *>(data);
    launch_app(app);
}

// Callback to open config files
void on_edit_config_clicked(GtkWidget *widget, gpointer data) {
    const char *config_file = static_cast<const char *>(data);
    edit_config(config_file);
}

// Main function with restored tabs and corrections
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
    GtkWidget *launcher_button = gtk_button_new_with_label("Launcher");
    GtkWidget *browser_button = gtk_button_new_with_label("Browser");
    GtkWidget *obs_button = gtk_button_new_with_label("OBS");
    GtkWidget *nautilus_button = gtk_button_new_with_label("Files");
    GtkWidget *terminal_button = gtk_button_new_with_label("Terminal");
        
    // Connect buttons to commands
    g_signal_connect(launcher_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "steam");
    g_signal_connect(browser_button, "clicked", G_CALLBACK(on_browser_clicked), NULL);
    g_signal_connect(obs_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "obs");
    g_signal_connect(nautilus_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "nautilus");
    g_signal_connect(terminal_button, "clicked", G_CALLBACK(on_terminal_clicked), NULL);
    
    // Add buttons to the Applications tab
    gtk_box_pack_start(GTK_BOX(applications_box), launcher_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), browser_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), obs_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), nautilus_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), terminal_button, TRUE, TRUE, 0);
    
    // Add Applications tab to the notebook
    GtkWidget *applications_label = gtk_label_new("Applications");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), applications_box, applications_label);

    // ==== Edit Configs Tab ====
    GtkWidget *configs_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *hyprland_button = gtk_button_new_with_label("Hyprland");
    GtkWidget *hyprpaper_button = gtk_button_new_with_label("Hyprpaper");
    GtkWidget *waybar_config_button = gtk_button_new_with_label("WayConfig");
    GtkWidget *waybar_style_button = gtk_button_new_with_label("WayStyle");
    GtkWidget *kitty_button_config = gtk_button_new_with_label("Kitty");

    // Connect buttons to edit config files with vim
    g_signal_connect(hyprland_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/hypr/hyprland.conf");
    g_signal_connect(hyprpaper_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/hypr/hyprpaper.conf");
    g_signal_connect(waybar_config_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/waybar/config");
    g_signal_connect(waybar_style_button, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/waybar/style.css");
    g_signal_connect(kitty_button_config, "clicked", G_CALLBACK(on_edit_config_clicked), (gpointer) ".config/kitty/kitty.conf");

    // Add config buttons to the Edit Configs tab
    gtk_box_pack_start(GTK_BOX(configs_box), hyprland_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), hyprpaper_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), waybar_config_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), waybar_style_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(configs_box), kitty_button_config, TRUE, TRUE, 0);

    // Add Edit Configs tab to the notebook
    GtkWidget *configs_label = gtk_label_new("Edit Configs");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), configs_box, configs_label);

    // Remaining tabs...

    // Show everything
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

