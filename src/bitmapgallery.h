#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

#include <vector>

enum class BitmapScaling : int
{
    Center = 0,
    Fit,
    FillWidth,
    FillHeight
};

class BitmapGallery : public wxWindow
{
public:
    BitmapGallery(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0)
        : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE | style)
    {
        this->SetBackgroundStyle(wxBG_STYLE_PAINT); // needed for windows

        this->Bind(wxEVT_PAINT, &BitmapGallery::OnPaint, this);

        this->Bind(wxEVT_KEY_DOWN, &BitmapGallery::OnKeyDown, this);
        this->Bind(wxEVT_LEFT_DOWN, &BitmapGallery::OnLeftDown, this);
        this->Bind(wxEVT_LEFT_DCLICK, &BitmapGallery::OnLeftDown, this);
    }

    void OnPaint(wxPaintEvent &evt)
    {
        wxAutoBufferedPaintDC dc(this);
        dc.Clear();

        wxGraphicsContext *gc = wxGraphicsContext::Create(dc);

        if (gc && bitmaps.size() > 0)
        {
            const wxSize drawSize = GetClientSize();

            DrawBitmaps(gc, drawSize);

            double arrowLineLength = NavigationRectSize().GetWidth() * 2 / 3;
            double arrowLineWidth = FromDIP(5);

            DrawNavigationRect(gc, NavigationRectLeft());
            DrawArrow(gc, NavigationRectLeft(), arrowLineLength, arrowLineWidth, 0);

            DrawNavigationRect(gc, NavigationRectRight());
            DrawArrow(gc, NavigationRectRight(), arrowLineLength, arrowLineWidth, M_PI);

            delete gc;
        }
    }

    void DrawBitmaps(wxGraphicsContext *gc, const wxSize &drawSize)
    {
        const auto currentTransform = gc->GetTransform();
        const wxSize dipDrawSize = ToDIP(drawSize);

        gc->Translate(-FromDIP(dipDrawSize.GetWidth()) * selectedIndex, 0);

        for (const auto &bitmap : bitmaps)
        {
            const wxSize bmpSize = bitmap.GetSize();

            // treating image size as DIP
            double imageW = bmpSize.GetWidth();
            double imageH = bmpSize.GetHeight();

            if (scaling == BitmapScaling::Fit)
            {
                double scaleX = dipDrawSize.GetWidth() / imageW;
                double scaleY = dipDrawSize.GetHeight() / imageH;

                double scale = std::min(scaleX, scaleY);

                imageW *= scale;
                imageH *= scale;
            }
            else if (scaling == BitmapScaling::FillWidth)
            {
                double scaleX = dipDrawSize.GetWidth() / imageW;

                imageW *= scaleX;
                imageH *= scaleX;
            }
            else if (scaling == BitmapScaling::FillHeight)
            {
                double scaleY = dipDrawSize.GetHeight() / imageH;

                imageW *= scaleY;
                imageH *= scaleY;
            }

            double cellCenterX = dipDrawSize.GetWidth() / 2;
            double imageCenterX = imageW / 2;

            double cellCenterY = dipDrawSize.GetHeight() / 2;
            double imageCenterY = imageH / 2;

            double bitmapX = cellCenterX - imageCenterX;
            double bitmapY = cellCenterY - imageCenterY;

            gc->Clip(0, 0, FromDIP(dipDrawSize.GetWidth()), FromDIP(dipDrawSize.GetHeight()));
            gc->DrawBitmap(bitmap, FromDIP(bitmapX), FromDIP(bitmapY), FromDIP(imageW), FromDIP(imageH));

            gc->ResetClip();

            gc->Translate(FromDIP(dipDrawSize.GetWidth()), 0);
        }

        gc->SetTransform(currentTransform);
    }

    void DrawNavigationRect(wxGraphicsContext *gc, const wxRect &rect)
    {
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->SetBrush(wxBrush(wxColor(255, 255, 255, 64)));

        gc->DrawRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight());
    }

    void DrawArrow(wxGraphicsContext *gc, const wxRect &rectToCenterIn, double lineLength, double lineWidth, double rotationAngle)
    {
        const auto currentTransform = gc->GetTransform();
        const auto rectCenter = rectToCenterIn.GetPosition() + rectToCenterIn.GetSize() / 2;

        gc->SetPen(wxPen(wxColor(255, 255, 255, 255), lineWidth));

        gc->Translate(rectCenter.x, rectCenter.y);
        gc->Rotate(-M_PI / 4);
        gc->Rotate(rotationAngle);
        gc->Translate(-lineLength / 4, -lineLength / 4);

        gc->StrokeLine(0, 0, lineLength, 0);
        gc->StrokeLine(0, 0, 0, lineLength);

        gc->SetTransform(currentTransform);
    }

    void OnKeyDown(wxKeyEvent &evt)
    {
        if (evt.GetKeyCode() == WXK_LEFT)
        {
            selectedIndex = std::max(0, selectedIndex - 1);
            Refresh();
        }
        else if (evt.GetKeyCode() == WXK_RIGHT)
        {
            selectedIndex = std::min(static_cast<int>(bitmaps.size()) - 1, selectedIndex + 1);
            Refresh();
        }
        else
        {
            evt.Skip();
        }
    }

    void OnLeftDown(wxMouseEvent &evt)
    {
        if (NavigationRectLeft().Contains(evt.GetPosition()))
        {
            selectedIndex = std::max(0, selectedIndex - 1);
            Refresh();
        }
        else if (NavigationRectRight().Contains(evt.GetPosition()))
        {
            selectedIndex = std::min(static_cast<int>(bitmaps.size()) - 1, selectedIndex + 1);
            Refresh();
        }
        else
        {
            evt.Skip();
        }
    }

    std::vector<wxBitmap> bitmaps;
    BitmapScaling scaling = BitmapScaling::Center;

private:
    int selectedIndex = 0;

    wxSize NavigationRectSize()
    {
        return {FromDIP(30), GetClientSize().GetHeight()};
    }

    wxRect NavigationRectLeft()
    {
        return {0, 0, NavigationRectSize().GetWidth(), NavigationRectSize().GetHeight()};
    }

    wxRect NavigationRectRight()
    {
        return {GetClientSize().GetWidth() - NavigationRectSize().GetWidth(), 0, NavigationRectSize().GetWidth(), NavigationRectSize().GetHeight()};
    }
};