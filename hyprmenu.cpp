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

// Function to run system update using pacman
void system_update() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: run the system update
        execlp("kitty", "kitty", "-e", "sudo", "pacman", "-Syu", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
}

// Function to launch nwg-look for GTK appearance settings
void launch_gtk_appearance() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: launch nwg-look
        execlp("nwg-look", "nwg-look", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
}

// Function to launch qt6ct for QT appearance settings
void launch_qt_appearance() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: launch qt6ct
        execlp("qt6ct", "qt6ct", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
}

// Callback for launching pavucontrol (volume control)
void launch_volume() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: launch pavucontrol
        execlp("pavucontrol", "pavucontrol", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
}

// Callback for launching gnome-disks (disk management)
void launch_disk_management() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: launch gnome-disks
        execlp("gnome-disks", "gnome-disks", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
}

// Callback for launching gnome-system-monitor (task manager)
void launch_task_manager() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: launch gnome-system-monitor
        execlp("gnome-system-monitor", "gnome-system-monitor", (char *)NULL);
        _exit(EXIT_FAILURE);
    }
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

// Callback for system update button
void on_system_update_clicked(GtkWidget *widget, gpointer data) {
    system_update();
}

// Callback for GTK appearance button
void on_gtk_appearance_clicked(GtkWidget *widget, gpointer data) {
    launch_gtk_appearance();
}

// Callback for QT appearance button
void on_qt_appearance_clicked(GtkWidget *widget, gpointer data) {
    launch_qt_appearance();
}

// Callback for volume button
void on_volume_clicked(GtkWidget *widget, gpointer data) {
    launch_volume();
}

// Callback for disk management button
void on_disk_management_clicked(GtkWidget *widget, gpointer data) {
    launch_disk_management();
}

// Callback for task manager button
void on_task_manager_clicked(GtkWidget *widget, gpointer data) {
    launch_task_manager();
}

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
    GtkWidget *steam_button = gtk_button_new_with_label("Steam");
    GtkWidget *firefox_button = gtk_button_new_with_label("Firefox");
    GtkWidget *obs_button = gtk_button_new_with_label("OBS");
    GtkWidget *nautilus_button = gtk_button_new_with_label("Files");
    GtkWidget *kitty_button = gtk_button_new_with_label("Kitty");
        
    // Connect buttons to commands (non-blocking)
    g_signal_connect(steam_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "steam");
    g_signal_connect(firefox_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "firefox");
    g_signal_connect(obs_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "obs-studio");
    g_signal_connect(nautilus_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "nautilus");
    g_signal_connect(kitty_button, "clicked", G_CALLBACK(on_button_clicked), (gpointer) "kitty");
    

    // Add buttons to the Applications tab
    gtk_box_pack_start(GTK_BOX(applications_box), steam_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), firefox_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), obs_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), nautilus_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(applications_box), kitty_button, TRUE, TRUE, 0);
     
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

    // ==== System Information Tab ====
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *system_info_label = gtk_label_new("CPU: i5-7600k @4.6GHz\n"
                                                 "GPU: RX580 (8GB)\n"
                                                 "OS: Arch Linux Zen");
    gtk_box_pack_start(GTK_BOX(info_box), system_info_label, TRUE, TRUE, 0);

    // Add System Information tab to the notebook
    GtkWidget *info_label = gtk_label_new("System Information");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), info_box, info_label);

    // ==== System Management Tab ====
    GtkWidget *system_management_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *volume_button = gtk_button_new_with_label("Volume");
    GtkWidget *disk_management_button = gtk_button_new_with_label("Disk Management");
    GtkWidget *task_manager_button = gtk_button_new_with_label("Task Manager");
    GtkWidget *gtk_appearance_button = gtk_button_new_with_label("GTK Appearance");
    GtkWidget *qt_appearance_button = gtk_button_new_with_label("QT Appearance");
    GtkWidget *update_button = gtk_button_new_with_label("Update System");

    // Connect buttons to system management commands
    g_signal_connect(volume_button, "clicked", G_CALLBACK(on_volume_clicked), NULL);
    g_signal_connect(disk_management_button, "clicked", G_CALLBACK(on_disk_management_clicked), NULL);
    g_signal_connect(task_manager_button, "clicked", G_CALLBACK(on_task_manager_clicked), NULL);
    g_signal_connect(gtk_appearance_button, "clicked", G_CALLBACK(on_gtk_appearance_clicked), NULL);
    g_signal_connect(qt_appearance_button, "clicked", G_CALLBACK(on_qt_appearance_clicked), NULL);
    g_signal_connect(update_button, "clicked", G_CALLBACK(on_system_update_clicked), NULL);


    // Add buttons to System Management tab
    gtk_box_pack_start(GTK_BOX(system_management_box), volume_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(system_management_box), disk_management_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(system_management_box), task_manager_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(system_management_box), gtk_appearance_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(system_management_box), qt_appearance_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(system_management_box), update_button, TRUE, TRUE, 0);


    // Add System Management tab to the notebook
    GtkWidget *system_management_label = gtk_label_new("System Management");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), system_management_box, system_management_label);

    // Show everything
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

