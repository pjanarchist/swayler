#include "theme_manager.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
json Theme::to_json() const {
    json j;
    j["name"] = name;
    j["gtk_theme"] = gtk_theme;
    j["icon_theme"] = icon_theme;
    j["cursor_theme"] = cursor_theme;
    j["color_scheme"] = color_scheme;
    
    if (wallpaper) {
        j["wallpaper"] = wallpaper.value();
    }
    if (qt_theme) {
        j["qt_theme"] = qt_theme.value();
    }
    
    return j;
}

Theme Theme::from_json(const json& j) {
    Theme theme;
    theme.name = j.at("name");
    theme.gtk_theme = j.at("gtk_theme");
    theme.icon_theme = j.at("icon_theme");
    theme.cursor_theme = j.at("cursor_theme");
    theme.color_scheme = j.value("color_scheme", "dark");
    
    if (j.contains("wallpaper")) {
        theme.wallpaper = j["wallpaper"].get<std::string>();
    }
    if (j.contains("qt_theme")) {
        theme.qt_theme = j["qt_theme"].get<std::string>();
    }
    
    return theme;
}

ThemeManager::ThemeManager() {
    home_dir = fs::home_directory();
    config_dir = home_dir / ".config";
    themes_dir = home_dir / ".themes";
    icons_dir = home_dir / ".icons";
    presets_dir = config_dir / "swayler" / "presets";
    
    // Create directories if they don't exist
    fs::create_directories(themes_dir);
    fs::create_directories(icons_dir);
    fs::create_directories(presets_dir);
}

std::vector<std::string> ThemeManager::get_gtk_themes() const {
    std::vector<std::string> themes;
    
    // Scan user themes
    if (fs::exists(themes_dir)) {
        for (const auto& entry : fs::directory_iterator(themes_dir)) {
            if (entry.is_directory()) {
                auto gtk_path = entry.path() / "gtk-3.0";
                if (fs::exists(gtk_path)) {
                    themes.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Scan system themes
    fs::path system_themes = "/usr/share/themes";
    if (fs::exists(system_themes)) {
        for (const auto& entry : fs::directory_iterator(system_themes)) {
            if (entry.is_directory()) {
                auto gtk_path = entry.path() / "gtk-3.0";
                if (fs::exists(gtk_path)) {
                    themes.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Remove duplicates and sort
    std::sort(themes.begin(), themes.end());
    themes.erase(std::unique(themes.begin(), themes.end()), themes.end());
    
    return themes;
}

std::vector<std::string> ThemeManager::get_icon_themes() const {
    std::vector<std::string> icons;
    
    // Scan user icons
    if (fs::exists(icons_dir)) {
        for (const auto& entry : fs::directory_iterator(icons_dir)) {
            if (entry.is_directory()) {
                auto index_path = entry.path() / "index.theme";
                if (fs::exists(index_path)) {
                    icons.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Scan system icons
    fs::path system_icons = "/usr/share/icons";
    if (fs::exists(system_icons)) {
        for (const auto& entry : fs::directory_iterator(system_icons)) {
            if (entry.is_directory()) {
                auto index_path = entry.path() / "index.theme";
                if (fs::exists(index_path)) {
                    icons.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Remove duplicates and sort
    std::sort(icons.begin(), icons.end());
    icons.erase(std::unique(icons.begin(), icons.end()), icons.end());
    
    return icons;
}

std::vector<std::string> ThemeManager::get_cursor_themes() const {
    std::vector<std::string> cursors;
    
    // Scan user cursors
    auto user_icons = icons_dir;
    if (fs::exists(user_icons)) {
        for (const auto& entry : fs::directory_iterator(user_icons)) {
            if (entry.is_directory()) {
                auto cursor_path = entry.path() / "cursors";
                if (fs::exists(cursor_path)) {
                    cursors.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Scan system cursors
    fs::path system_cursors = "/usr/share/icons";
    if (fs::exists(system_cursors)) {
        for (const auto& entry : fs::directory_iterator(system_cursors)) {
            if (entry.is_directory()) {
                auto cursor_path = entry.path() / "cursors";
                if (fs::exists(cursor_path)) {
                    cursors.push_back(entry.path().filename().string());
                }
            }
        }
    }
    
    // Remove duplicates and sort
    std::sort(cursors.begin(), cursors.end());
    cursors.erase(std::unique(cursors.begin(), cursors.end()), cursors.end());
    
    return cursors;
}

bool ThemeManager::apply_theme(const Theme& theme) {
    try {
        apply_gtk(theme.gtk_theme, theme.icon_theme, theme.cursor_theme);
        apply_sway(theme);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error applying theme: " << e.what() << std::endl;
        return false;
    }
}

void ThemeManager::apply_gtk(const std::string& theme, const std::string& icons, const std::string& cursor) {
    auto gtk3_config = config_dir / "gtk-3.0" / "settings.ini";
    fs::create_directories(gtk3_config.parent_path());
    
    std::string gtk_content = "[Settings]\n";
    gtk_content += "gtk-theme-name=" + theme + "\n";
    gtk_content += "gtk-icon-theme-name=" + icons + "\n";
    gtk_content += "gtk-cursor-theme-name=" + cursor + "\n";
    
    std::ofstream file(gtk3_config);
    file << gtk_content;
    file.close();
    
    // Copy to GTK4
    auto gtk4_config = config_dir / "gtk-4.0" / "settings.ini";
    fs::create_directories(gtk4_config.parent_path());
    fs::copy_file(gtk3_config, gtk4_config, fs::copy_options::overwrite_existing);
}

void ThemeManager::apply_sway(const Theme& theme) {
    // TODO: Integrate with sway configuration
    // This would involve modifying ~/.config/sway/config
    // For now, just print what would be done
    std::cout << "Would apply to Sway:" << std::endl;
    std::cout << "  GTK: " << theme.gtk_theme << std::endl;
    std::cout << "  Icons: " << theme.icon_theme << std::endl;
    std::cout << "  Cursor: " << theme.cursor_theme << std::endl;
}

void ThemeManager::apply_generic(const Theme& theme) {
    apply_gtk(theme.gtk_theme, theme.icon_theme, theme.cursor_theme);
}

std::vector<std::string> ThemeManager::list_presets() const {
    std::vector<std::string> presets;
    
    if (!fs::exists(presets_dir)) {
        return presets;
    }
    
    for (const auto& entry : fs::directory_iterator(presets_dir)) {
        if (entry.path().extension() == ".json") {
            presets.push_back(entry.path().stem().string());
        }
    }
    
    std::sort(presets.begin(), presets.end());
    return presets;
}

std::optional<Theme> ThemeManager::load_preset(const std::string& name) const {
    auto preset_path = presets_dir / (name + ".json");
    
    if (!fs::exists(preset_path)) {
        return std::nullopt;
    }
    
    try {
        std::ifstream file(preset_path);
        json data;
        file >> data;
        return Theme::from_json(data);
    } catch (const std::exception& e) {
        std::cerr << "Error loading preset: " << e.what() << std::endl;
        return std::nullopt;
    }
}

bool ThemeManager::save_preset(const Theme& theme) {
    try {
        auto preset_path = presets_dir / (theme.name + ".json");
        std::ofstream file(preset_path);
        file << theme.to_json().dump(2);
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving preset: " << e.what() << std::endl;
        return false;
    }
}

bool ThemeManager::delete_preset(const std::string& name) {
    try {
        auto preset_path = presets_dir / (name + ".json");
        fs::remove(preset_path);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting preset: " << e.what() << std::endl;
        return false;
    }
}

Theme ThemeManager::get_current_theme() const {
    Theme current;
    current.name = "Current";
    current.gtk_theme = "Adwaita";
    current.icon_theme = "Adwaita";
    current.cursor_theme = "default";
    current.color_scheme = "dark";
    
    // TODO: Read actual current theme from settings
    
    return current;
}