#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <optional>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

struct Theme {
    std::string name;
    std::string gtk_theme;
    std::string icon_theme;
    std::string cursor_theme;
    std::optional<std::string> wallpaper;
    std::optional<std::string> qt_theme;
    std::string color_scheme;  // "dark" or "light"

    json to_json() const;
    static Theme from_json(const json& j);
};

class ThemeManager {
public:
    ThemeManager();

    // List available themes
    std::vector<std::string> get_gtk_themes() const;
    std::vector<std::string> get_icon_themes() const;
    std::vector<std::string> get_cursor_themes() const;

    // Apply themes
    bool apply_theme(const Theme& theme);

    // Preset management
    std::vector<std::string> list_presets() const;
    std::optional<Theme> load_preset(const std::string& name) const;
    bool save_preset(const Theme& theme);
    bool delete_preset(const std::string& name);

    // Get current theme state
    Theme get_current_theme() const;

private:
    fs::path home_dir;
    fs::path config_dir;
    fs::path themes_dir;
    fs::path icons_dir;
    fs::path presets_dir;

    // Apply to specific DEs
    void apply_gtk(const std::string& theme, const std::string& icons, const std::string& cursor);
    void apply_sway(const Theme& theme);
    void apply_generic(const Theme& theme);

    // Helper functions
    std::vector<std::string> scan_theme_dir(const fs::path& dir) const;
    std::string get_config_value(const std::string& key) const;
    void set_config_value(const std::string& key, const std::string& value);
};