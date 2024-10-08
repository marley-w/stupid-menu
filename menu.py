import os
import subprocess
import gi

# Import GTK 3
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

class SettingsApp(Gtk.Window):
    def __init__(self):
        super().__init__(title="Hyprland Settings")
        self.set_default_size(400, 300)

        # Create a notebook (tabbed interface)
        self.notebook = Gtk.Notebook()
        self.add(self.notebook)

        # Tab 1: Edit Configs
        self.edit_configs_page = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.notebook.append_page(self.edit_configs_page, Gtk.Label(label="Edit Configs"))

        self.config_files = [
            os.path.expanduser("~/.config/hypr/hyprland.conf"),
            os.path.expanduser("~/.config/hypr/hyprpaper.conf"),
            os.path.expanduser("~/.config/waybar/config"),
            os.path.expanduser("~/.config/waybar/style.css"),
            os.path.expanduser("~/.config/MangoHud/MangoHud.conf"),
            os.path.expanduser("~/.config/kitty/kitty.conf"),
            os.path.expanduser("~/.config/fastfetch/fastfetch.jsonc"),
        ]

        for config in self.config_files:
            button = Gtk.Button(label=os.path.basename(config))
            button.connect("clicked", self.on_edit_file_clicked, config)
            self.edit_configs_page.pack_start(button, True, True, 0)

        # Tab 2: Change Wallpaper
        self.wallpaper_page = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.notebook.append_page(self.wallpaper_page, Gtk.Label(label="Change Wallpaper"))

        # Placeholder for functionality
        placeholder_label = Gtk.Label(label="Change Wallpaper functionality will be added later.")
        self.wallpaper_page.pack_start(placeholder_label, True, True, 0)

        # Tab 3: Reboot
        self.reboot_page = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.notebook.append_page(self.reboot_page, Gtk.Label(label="Reboot"))

        reboot_button = Gtk.Button(label="Reboot System")
        reboot_button.connect("clicked", self.on_reboot_clicked)
        self.reboot_page.pack_start(reboot_button, True, True, 0)

        # Tab 4: Exit
        self.exit_page = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=6)
        self.notebook.append_page(self.exit_page, Gtk.Label(label="Exit"))

        exit_button = Gtk.Button(label="Exit")
        exit_button.connect("clicked", Gtk.main_quit)
        self.exit_page.pack_start(exit_button, True, True, 0)

    def on_edit_file_clicked(self, widget, config):
        if os.path.isfile(config):
            # Use Kitty terminal to open Vim
            subprocess.run(['kitty', 'vim', config])  # Open the config file with Vim in Kitty

    def on_reboot_clicked(self, widget):
        # Placeholder for reboot functionality
        print("Rebooting the system...")  # You could use subprocess.run(['sudo', 'reboot']) here

if __name__ == "__main__":
    app = SettingsApp()
    app.connect("destroy", Gtk.main_quit)
    app.show_all()  # Show the window
    Gtk.main()  # Start the GTK main loop

