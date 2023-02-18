#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>

class ChartControl : public wxWindow
{
public:
    ChartControl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

    std::vector<std::pair<double, double>> values;
    std::string title;

    std::pair<double, double> highlightedPoint;
    double minX, maxX;

private:
    void OnPaint(wxPaintEvent &evt);
    std::tuple<int, double, double> calculateChartSegmentCountAndRange(double origLow, double origHigh);
};