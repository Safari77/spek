#pragma once

#include <memory>

#include <wx/wx.h>

#include "spek-palette.h"
#include "spek-pipeline.h"

class Audio;
class FFT;
class SpekHaveSampleEvent;
struct spek_pipeline;

enum
{
    ID_RESIZE_TIMER = wxID_HIGHEST + 1,
    ID_UPDATE_TIMER
};

class SpekSpectrogram : public wxWindow
{
public:
    SpekSpectrogram(wxFrame *parent);
    ~SpekSpectrogram();
    void open(const wxString& path);
    void save(const wxString& path);

private:
    void on_char(wxKeyEvent& evt);
    void on_paint(wxPaintEvent& evt);
    void on_size(wxSizeEvent& evt);
    void on_resize_timer(wxTimerEvent& event);
    void on_update(wxTimerEvent& event);
    void on_have_sample(SpekHaveSampleEvent& evt);
    void render(wxDC& dc);

    void start();
    void stop();

    void create_palette();

    std::unique_ptr<Audio> audio;
    std::unique_ptr<FFT> fft;
    spek_pipeline *pipeline;
    int streams;
    int stream;
    int channels;
    int channel;
    enum window_function window_function;
    wxString path;
    wxString desc;
    double duration;
    int sample_rate;
    enum palette palette;
    wxImage palette_image;
    wxImage image;
    wxSize prev_size;
    wxSize prev_save_size;
    wxTimer m_resizeTimer;
    wxTimer m_updateTimer;
    int fft_bits;
    int urange;
    int lrange;
    const int LPAD;
    const int TPAD;
    const int RPAD;
    const int BPAD;
    const int GAP;
    const int RULER;

    DECLARE_EVENT_TABLE()
};
