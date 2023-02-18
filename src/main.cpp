#include <wx/wx.h>

#include "animator.h"
#include "animatedvalue.h"

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

private:
    wxPanel *item;

    wxButton *startButton;
    wxButton *resetButton;

    Animator animator;
    std::vector<AnimatedValue> animatedValues;

    void SetupAnimations();
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
    SetupAnimations();

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    auto animPanel = new wxPanel(this, wxID_ANY);
    animPanel->SetBackgroundColour(wxColour(100, 100, 200));

    item = new wxPanel(animPanel, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(100, 100)));
    item->SetBackgroundColour(wxColour(200, 100, 100));

    auto buttonsPanel = new wxPanel(this, wxID_ANY);
    auto buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    startButton = new wxButton(buttonsPanel, wxID_ANY, "Start");
    resetButton = new wxButton(buttonsPanel, wxID_ANY, "Reset");

    resetButton->Disable();

    buttonsSizer->Add(startButton, 0, wxALL, FromDIP(10));
    buttonsSizer->Add(resetButton, 0, wxALL, FromDIP(10));

    buttonsPanel->SetSizer(buttonsSizer);

    mainSizer->Add(animPanel, 1, wxEXPAND);
    mainSizer->Add(buttonsPanel, 0, wxEXPAND);

    mainSizer->SetMinSize(FromDIP(wxSize(500, 600)));

    this->SetSizerAndFit(mainSizer);

    startButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                      {
                          startButton->Disable();
                          resetButton->Disable();
                          animator.Start(1000); });

    resetButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &event)
                      {
                          animator.Reset();
                          startButton->Enable();
                          resetButton->Disable();
                          Refresh(); });

    animator.Reset();
}

void MyFrame::SetupAnimations()
{
    animatedValues = {
        {0, 300, [this](AnimatedValue *sender, double tNorm, double value)
         {
             item->SetPosition(wxPoint(FromDIP(value), item->GetPosition().y));
         }}};

    animator.SetAnimatedValues(animatedValues);
    animator.SetOnIteration([this]()
                            { Refresh(); });

    animator.SetOnStop([this]()
                       { resetButton->Enable(); });
}