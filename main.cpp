#include <wx/wx.h>
#include <wx/sizer.h>

#include "./julia.h"

class JuliaDrawPane : public wxPanel {
public:
  double view_center_x;
  double view_center_y;
  double view_width;
  double tmp_center_x;
  double tmp_center_y;
  JuliaDrawPane(wxWindow* parent)
    : wxPanel(parent) {
    view_center_x = 0;
    view_center_y = 0;
    tmp_center_x = 0;
    tmp_center_y = 0;
    view_width = 5;
    mouse_down = false;

    Bind(wxEVT_PAINT, &JuliaDrawPane::paint_event, this);
    Bind(wxEVT_LEFT_DOWN, &JuliaDrawPane::mouse_down_event, this);
    Bind(wxEVT_LEFT_UP, &JuliaDrawPane::mouse_release_event, this);
    Bind(wxEVT_MOUSEWHEEL, &JuliaDrawPane::mouse_wheel_event, this);
    Bind(wxEVT_MOTION, &JuliaDrawPane::mouse_move_event, this);
  }

  void render(wxDC* dc, wxImage* image) {
    wxBitmap* bitmap = new wxBitmap(*image);
    dc->DrawBitmap(*bitmap, 0, 0, false);
  }

  void paint_event(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxCoord width;
    wxCoord height;
    dc.GetSize(&width, &height);
    unsigned char* data = (unsigned char*) malloc(3 * sizeof(char) * width * height);

    render_julia_rgb(data, (unsigned int) width, (unsigned int) height, view_width, tmp_center_x, tmp_center_y, 100);

    /* wxImage will free data */
    wxImage image(width, height, data);
    render(&dc, &image);
  }

   void mouse_wheel_event(wxMouseEvent& event) {
     if (event.GetWheelAxis() == wxMOUSE_WHEEL_VERTICAL) {
       double delta = (double) event.GetWheelRotation() / event.GetWheelDelta();
       /* Todo make scroll at cursor position */
       view_width *= pow(0.8, delta);
       this->Refresh();
     }
   }

   wxCoord start_x;
   wxCoord start_y;
   bool mouse_down;
   void mouse_down_event(wxMouseEvent& event) {
     start_x = event.GetX();
     start_y = event.GetY();
     mouse_down = true;
   }
   void mouse_release_event(wxMouseEvent& event) {
     int window_width;
     GetSize(&window_width, NULL);
     view_center_x += -view_width / window_width * (event.GetX() - start_x);
     view_center_y += +view_width / window_width * (event.GetY() - start_y);
     tmp_center_x = view_center_x;
     tmp_center_y = view_center_y;
     mouse_down = false;
     this->Refresh();
   }
   void mouse_move_event(wxMouseEvent& event) {
     if (mouse_down) {
       int window_width;
       GetSize(&window_width, NULL);
       tmp_center_x = view_center_x - view_width / window_width * (event.GetX() - start_x);
       tmp_center_y = view_center_y + view_width / window_width * (event.GetY() - start_y);
       this->Refresh();
     }
   }

   /*
   void rightClick(wxMouseEvent& event);
   void mouseLeftWindow(wxMouseEvent& event);
   void keyPressed(wxKeyEvent& event);
   void keyReleased(wxKeyEvent& event);
   */
};

class JuliaWxMainFrame : public wxFrame {
public:
  JuliaWxMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    JuliaDrawPane* pane = new JuliaDrawPane(this);
    pane->Show(true);
  }
};

class JuliaMapWxApp : public wxApp {
public:
  bool OnInit() {
    JuliaWxMainFrame* frame = new JuliaWxMainFrame(wxT("Julia Map"), wxDefaultPosition, wxDefaultSize);
    frame->Show(true);
    return true;
  }
};
wxIMPLEMENT_APP_NO_MAIN(JuliaMapWxApp);

void create_wx_window(void) {
  int argc = 0;
  char** argv = NULL;
  wxEntry(argc, argv);
}

int main(int argc, char** argv) {
  create_wx_window();
  return 0;
}
