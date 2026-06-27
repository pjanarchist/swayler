#pragma once

#include <gtkmm.h>
#include <libadwaita-1/adwaita.h>
#include <memory>

class SwaylerWindow;

class SwaylerApplication : public Gtk::Application {
public:
    SwaylerApplication();
    virtual ~SwaylerApplication();

    static Glib::RefPtr<SwaylerApplication> create();

protected:
    void on_activate() override;

private:
    std::unique_ptr<SwaylerWindow> m_window;
};