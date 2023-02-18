
#include <wx/wx.h>

#include "bitmapgallery.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    wxInitAllImageHandlers();

    auto gallery = new BitmapGallery(this);

    wxImage image("/path/to/your/image.jpeg");
    gallery->bitmap = image;
    gallery->scaling = BitmapScaling::Fit;
}
