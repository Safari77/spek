#pragma once

#include <wx/fileconf.h>
#include <wx/intl.h>
#include <wx/window.h>
#include <wx/dcbuffer.h>

class SpekPreferences
{
public:
    static SpekPreferences& get();

    void init();
    bool get_check_update();
    void set_check_update(bool value);
    long get_last_update();
    void set_last_update(long value);
    wxString get_language();
    void set_language(const wxString& value);
    void set_window_size(int width, int height);
    wxSize get_window_size();

    static const int DEF_WIDTH = 800;
    static const int DEF_HEIGHT = 600;

private:
    SpekPreferences();
    SpekPreferences(const SpekPreferences&);
    void operator=(const SpekPreferences&);

    wxLocale *locale;
    wxFileConfig *config;
};
