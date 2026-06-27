#include "window.hpp"
#include "application.hpp"
#include <iostream>

// ThemeCard implementation
ThemeCard::ThemeCard(const std::string& title, const std::vector<std::string>& options)
    : Gtk::Box(Gtk::Orientation::VERTICAL, 8),
      m_title(title),
      m_model(Gtk::StringList::create(options)) {
    
    m_title.set_halign(Gtk::Align::START);
    m_title.add_css_class("title-4");
    
    m_dropdown.set_model(m_model);
    m_dropdown.set_hexpand(true);
    m_dropdown.set_halign(Gtk::Align::FILL);
    
    append(m_title);
    append(m_dropdown);
    set_margin_start(12);
    set_margin_end(12);
    set_margin_top(12);
    set_margin_bottom(12);
}

std::string ThemeCard::get_selected() const {
    if (m_dropdown.get_selected() == GTK_INVALID_LIST_POSITION) {
        return "";
    }
    auto item = m_model->get_string(m_dropdown.get_selected());
    return std::string(item);
}

void ThemeCard::set_selected(const std::string& value) {
    for (guint i = 0; i < m_model->get_n_items(); i++) {
        if (m_model->get_string(i) == value) {
            m_dropdown.set_selected(i);
            return;
        }
    }
}

// SwaylerWindow implementation
SwaylerWindow::SwaylerWindow(SwaylerApplication& app)
    : Gtk::ApplicationWindow(),
      m_app(&app),
      m_theme_manager(std::make_unique<ThemeManager>()),
      m_main_box(Gtk::Orientation::VERTICAL, 0),
      m_apply_button("Apply Theme"),
      m_save_preset_button("Save Preset"),
      m_load_preset_button("Load"),
      m_delete_preset_button("Delete") {
    
    set_default_size(500, 700);
    set_title("Swayler");
    
    setup_ui();
    load_themes();
}

SwaylerWindow::~SwaylerWindow() = default;

void SwaylerWindow::setup_ui() {
    auto root_box = Gtk::Box(Gtk::Orientation::VERTICAL);
    
    // Create scrollable content area
    auto scrolled = Gtk::ScrolledWindow();
    auto content_box = Gtk::Box(Gtk::Orientation::VERTICAL, 16);
    content_box.set_margin_start(12);
    content_box.set_margin_end(12);
    content_box.set_margin_top(12);
    content_box.set_margin_bottom(12);
    
    // Theme selection section
    auto theme_section = Gtk::Box(Gtk::Orientation::VERTICAL, 12);
    auto theme_title = Gtk::Label("Theme");
    theme_title.add_css_class("title-3");
    theme_title.set_halign(Gtk::Align::START);
    theme_section.append(theme_title);
    
    // Create theme cards
    auto gtk_themes = m_theme_manager->get_gtk_themes();
    auto icon_themes = m_theme_manager->get_icon_themes();
    auto cursor_themes = m_theme_manager->get_cursor_themes();
    
    m_gtk_card = std::make_unique<ThemeCard>("GTK Theme", gtk_themes);
    m_icon_card = std::make_unique<ThemeCard>("Icon Theme", icon_themes);
    m_cursor_card = std::make_unique<ThemeCard>("Cursor Theme", cursor_themes);
    
    theme_section.append(*m_gtk_card);
    theme_section.append(*m_icon_card);
    theme_section.append(*m_cursor_card);
    
    // Apply button
    m_apply_button.add_css_class("suggested-action");
    m_apply_button.set_halign(Gtk::Align::CENTER);
    m_apply_button.signal_clicked().connect([this]() {
        on_apply_clicked();
    });
    theme_section.append(m_apply_button);
    
    content_box.append(theme_section);
    
    // Separator
    auto separator1 = Gtk::Separator();
    content_box.append(separator1);
    
    // Presets section
    auto presets_section = Gtk::Box(Gtk::Orientation::VERTICAL, 12);
    auto presets_title = Gtk::Label("Presets");
    presets_title.add_css_class("title-3");
    presets_title.set_halign(Gtk::Align::START);
    presets_section.append(presets_title);
    
    // Presets list
    m_presets_list.set_selection_mode(Gtk::SelectionMode::SINGLE);
    m_presets_list.set_vexpand(true);
    presets_section.append(m_presets_list);
    
    // Preset buttons
    auto button_box = Gtk::Box(Gtk::Orientation::HORIZONTAL, 8);
    m_save_preset_button.set_hexpand(true);
    m_load_preset_button.set_hexpand(true);
    m_delete_preset_button.set_hexpand(true);
    
    m_save_preset_button.signal_clicked().connect([this]() {
        on_save_preset_clicked();
    });
    m_load_preset_button.signal_clicked().connect([this]() {
        on_load_preset_clicked();
    });
    m_delete_preset_button.signal_clicked().connect([this]() {
        on_delete_preset_clicked();
    });
    
    button_box.append(m_save_preset_button);
    button_box.append(m_load_preset_button);
    button_box.append(m_delete_preset_button);
    presets_section.append(button_box);
    
    content_box.append(presets_section);
    
    scrolled.set_child(content_box);
    scrolled.set_vexpand(true);
    
    root_box.append(scrolled);
    
    set_child(root_box);
}

void SwaylerWindow::load_themes() {
    auto presets = m_theme_manager->list_presets();
    for (const auto& preset : presets) {
        auto row = Gtk::ListBoxRow();
        auto label = Gtk::Label(preset);
        label.set_halign(Gtk::Align::START);
        row.set_child(label);
        m_presets_list.append(row);
    }
}

void SwaylerWindow::on_apply_clicked() {
    Theme theme;
    theme.name = "Custom";
    theme.gtk_theme = m_gtk_card->get_selected();
    theme.icon_theme = m_icon_card->get_selected();
    theme.cursor_theme = m_cursor_card->get_selected();
    theme.color_scheme = "dark";
    
    if (m_theme_manager->apply_theme(theme)) {
        std::cout << "Theme applied successfully!" << std::endl;
    } else {
        std::cerr << "Failed to apply theme" << std::endl;
    }
}

void SwaylerWindow::on_save_preset_clicked() {
    std::cout << "Save preset clicked" << std::endl;
}

void SwaylerWindow::on_load_preset_clicked() {
    std::cout << "Load preset clicked" << std::endl;
}

void SwaylerWindow::on_delete_preset_clicked() {
    std::cout << "Delete preset clicked" << std::endl;
}