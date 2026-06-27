#include "application.hpp"
#include "window.hpp"

SwaylerApplication::SwaylerApplication()
    : Gtk::Application("com.github.swayler",
                       Gio::APPLICATION_DEFAULT_FLAGS) {
}

SwaylerApplication::~SwaylerApplication() = default;

Glib::RefPtr<SwaylerApplication> SwaylerApplication::create() {
    return Glib::make_refptr(new SwaylerApplication());
}

void SwaylerApplication::on_activate() {
    if (!m_window) {
        m_window = std::make_unique<SwaylerWindow>(*this);
    }
    m_window->present();
}
