#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define MAX_IMAGES 100

// Define color codes
#define PURPLE "\e[1;35m"
#define RESET "\e[0m"

// Function to display system information
void show_system_info() {
    printf(PURPLE "Fetching system information...\n" RESET);
    system("fastfetch");
    printf(PURPLE "Press any key to continue...\n" RESET);
    getchar();
}

// Function to display images from ~/Pictures
void show_pictures() {
    DIR *dir;
    struct dirent *entry;
    char *image_files[MAX_IMAGES];
    int count = 0;

    // Open the Pictures directory
    dir = opendir("/home/marley/Pictures"); // Updated to "marley"
    if (dir == NULL) {
        printf(PURPLE "Could not open ~/Pictures directory.\n" RESET);
        return;
    }

    // Read image files
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".jpg") || strstr(entry->d_name, ".jpeg") || 
            strstr(entry->d_name, ".png") || strstr(entry->d_name, ".gif")) {
            image_files[count] = malloc(strlen(entry->d_name) + 1);
            strcpy(image_files[count], entry->d_name);
            count++;
        }
    }
    closedir(dir);

    if (count == 0) {
        printf(PURPLE "No images found in ~/Pictures.\n" RESET);
        return;
    }

    // Display image options
    printf(PURPLE "Choose an image to view:\n" RESET);
    for (int i = 0; i < count; i++) {
        printf(PURPLE "%d) %s\n" RESET, i + 1, image_files[i]);
    }

    int choice;
    printf(PURPLE "Enter the number of the image to view (or 0 to quit): " RESET);
    scanf("%d", &choice);
    getchar(); // Consume newline character

    if (choice > 0 && choice <= count) {
        char command[256];
        snprintf(command, sizeof(command), "feh /home/marley/Pictures/%s", image_files[choice - 1]); // Updated to "marley"
        system(command);
    }

    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(image_files[i]);
    }
}

// Function to edit the Hyprland configuration
void edit_hyprland_conf() {
    printf(PURPLE "Opening Hyprland configuration...\n" RESET);
    system("sudo vim ~/.config/hypr/hyprland.conf");
}

int main() {
    int choice;

    while (1) {
        printf(PURPLE "Menu:\n" RESET);
        printf(PURPLE "1) System Information\n" RESET);
        printf(PURPLE "2) View Pictures\n" RESET);
        printf(PURPLE "3) Edit Hyprland Config\n" RESET);
        printf(PURPLE "4) Exit\n" RESET);
        printf(PURPLE "Enter your choice: " RESET);
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1:
                show_system_info();
                break;
            case 2:
                show_pictures();
                break;
            case 3:
                edit_hyprland_conf();
                break;
            case 4:
                exit(0);
            default:
                printf(PURPLE "Invalid option. Please try again.\n" RESET);
                break;
        }
    }

    return 0;
}

