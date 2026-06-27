#include "application.hpp"
#include "window.hpp"

SwaylerApplication::SwaylerApplication()
    : Gtk::Application("com.github.swayler") {
}

SwaylerApplication::~SwaylerApplication() = default;

Glib::RefPtr<SwaylerApplication> SwaylerApplication::create() {
    return Glib::RefPtr<SwaylerApplication>(new SwaylerApplication());
}

void SwaylerApplication::on_activate() {
    if (!m_window) {
        m_window = std::make_unique<SwaylerWindow>(*this);
    }
    m_window->present();
}