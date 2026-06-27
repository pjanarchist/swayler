#pragma once

#include <gtkmm.h>
#include <adwaita.h>
#include <memory>
#include <vector>
#include "theme_manager.hpp"

class SwaylerApplication;

class ThemeCard : public Adw::Bin {
public:
    ThemeCard(const std::string& title, const std::vector<std::string>& options);
    
    std::string get_selected() const;
    void set_selected(const std::string& value);

private:
    Gtk::Box m_box;
    Gtk::Label m_title;
    Gtk::DropDown m_dropdown;
    Glib::RefPtr<Gtk::StringList> m_model;
};

class SwaylerWindow : public Adw::ApplicationWindow {
public:
    SwaylerWindow(SwaylerApplication& app);
    virtual ~SwaylerWindow();

private:
    void setup_ui();
    void load_themes();
    void on_apply_clicked();
    void on_save_preset_clicked();
    void on_load_preset_clicked();
    void on_delete_preset_clicked();

    SwaylerApplication* m_app;
    std::unique_ptr<ThemeManager> m_theme_manager;

    // UI Components
    Adw::HeaderBar m_header_bar;
    Gtk::Box m_main_box;
    
    // Theme cards
    std::unique_ptr<ThemeCard> m_gtk_card;
    std::unique_ptr<ThemeCard> m_icon_card;
    std::unique_ptr<ThemeCard> m_cursor_card;
    
    // Buttons
    Gtk::Button m_apply_button;
    Gtk::Button m_save_preset_button;
    Gtk::Button m_load_preset_button;
    Gtk::Button m_delete_preset_button;
    
    // Presets list
    Gtk::ListBox m_presets_list;
};