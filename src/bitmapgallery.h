#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

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

        if (gc && bitmap.IsOk())
        {
            const wxSize drawSize = GetClientSize();

            DrawBitmap(gc, drawSize);
            delete gc;
        }
    }

    void DrawBitmap(wxGraphicsContext *gc, const wxSize &drawSize)
    {
        const wxSize dipDrawSize = ToDIP(drawSize);
        const wxSize bmpSize = bitmap.GetSize();

        // treating image size as DIP
        double imageW = bmpSize.GetWidth();
        double imageH = bmpSize.GetHeight();

        double cellCenterX = dipDrawSize.GetWidth() / 2;
        double imageCenterX = imageW / 2;

        double cellCenterY = dipDrawSize.GetHeight() / 2;
        double imageCenterY = imageH / 2;

        double bitmapX = cellCenterX - imageCenterX;
        double bitmapY = cellCenterY - imageCenterY;

        gc->DrawBitmap(bitmap, FromDIP(bitmapX), FromDIP(bitmapY), FromDIP(imageW), FromDIP(imageH));
    }

    wxBitmap bitmap;
};