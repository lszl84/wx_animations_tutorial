
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

    gallery->scaling = BitmapScaling::FillWidth;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN, "&Open...\tCtrl-O", "Open a file");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");

    this->SetMenuBar(menuBar);

    this->Bind(
        wxEVT_MENU, [=](wxCommandEvent &evt)
        {
            wxFileDialog openFileDialog(this, "Open image", "", "",
                                        "Image files (*.png;*.jpeg;*.jpg)|*.png;*.jpeg;*.jpg", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
            if (openFileDialog.ShowModal() == wxID_CANCEL)
                return;

            wxArrayString paths;
            openFileDialog.GetPaths(paths);

            std::vector<wxBitmap> bitmaps;
            for (auto &path : paths)
            {
                bitmaps.push_back({wxImage(path)});
            }

            gallery->bitmaps = bitmaps;
            gallery->Refresh(); },
        wxID_OPEN);
}
