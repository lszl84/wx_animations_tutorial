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
            delete gc;
        }
    }

    void DrawBitmaps(wxGraphicsContext *gc, const wxSize &drawSize)
    {
        const auto currentTransform = gc->GetTransform();
        const wxSize dipDrawSize = ToDIP(drawSize / static_cast<int>(bitmaps.size()));

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

            // temporary overlay
            gc->SetPen(*wxRED_PEN);
            gc->SetBrush(wxColor(255, 0, 0, 64));
            gc->DrawRectangle(0, 0, FromDIP(dipDrawSize.GetWidth()), FromDIP(dipDrawSize.GetHeight()));

            gc->Translate(FromDIP(dipDrawSize.GetWidth()), 0);
        }

        gc->SetTransform(currentTransform);
    }

    std::vector<wxBitmap> bitmaps;
    BitmapScaling scaling = BitmapScaling::Center;
};