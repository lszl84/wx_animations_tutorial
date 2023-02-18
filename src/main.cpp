#include <wx/wx.h>

#include "animator.h"
#include "animatedvalue.h"

#include "chartcontrol.h"

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

    std::vector<ChartControl *> charts;

    void SetupAnimations();
    void SetupCharts();
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

    SetupCharts();

    auto buttonsPanel = new wxPanel(this, wxID_ANY);
    auto buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    startButton = new wxButton(buttonsPanel, wxID_ANY, "Start");
    resetButton = new wxButton(buttonsPanel, wxID_ANY, "Reset");

    resetButton->Disable();

    buttonsSizer->Add(startButton, 0, wxALL, FromDIP(10));
    buttonsSizer->Add(resetButton, 0, wxALL, FromDIP(10));

    buttonsPanel->SetSizer(buttonsSizer);

    mainSizer->Add(animPanel, 1, wxEXPAND);

    for (const auto &chart : charts)
    {
        mainSizer->Add(chart, 1, wxEXPAND);
    }

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

             auto x = tNorm;
             auto y = sender->easingFunction(sender->startValue, sender->endValue, tNorm);
             charts[0]->highlightedPoint = {x, y};
         },
         "Horizontal Position", AnimatedValue::Linear},
        {100, 255, [this](AnimatedValue *sender, double tNorm, double value)
         {
             item->SetBackgroundColour(wxColour(200, value, 100));

             auto x = tNorm;
             auto y = sender->easingFunction(sender->startValue, sender->endValue, tNorm);
             charts[1]->highlightedPoint = {x, y};
         },
         "Green Color Value", AnimatedValue::Linear}};

    animator.SetAnimatedValues(animatedValues);
    animator.SetOnIteration([this]()
                            { Refresh(); });

    animator.SetOnStop([this]()
                       { resetButton->Enable(); });
}

void MyFrame::SetupCharts()
{
    for (const auto &animatedValue : animatedValues)
    {
        auto chart = new ChartControl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        chart->title = animatedValue.description;

        chart->minX = 0;
        chart->maxX = 1;

        auto pointsCount = 100;

        for (int i = 0; i < pointsCount; i++)
        {
            auto x = static_cast<double>(i) / (pointsCount - 1);
            auto y = animatedValue.easingFunction(animatedValue.startValue, animatedValue.endValue, x);
            chart->values.push_back({x, y});
        }

        charts.push_back(chart);
    }
}