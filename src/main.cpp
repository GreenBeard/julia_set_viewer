#include <wx/wx.h>
#include <wx/sizer.h>

#include "./julia.h"

class SetDrawPane : public wxPanel {
public:
  double view_center_x;
  double view_center_y;
  double view_width;
  double tmp_center_x;
  double tmp_center_y;
  SetDrawPane(wxWindow* parent)
    : wxPanel(parent) {
    view_center_x = 0;
    view_center_y = 0;
    tmp_center_x = 0;
    tmp_center_y = 0;
    view_width = 5;
    mouse_down = false;

    Bind(wxEVT_PAINT, &SetDrawPane::paint_event, this);
    Bind(wxEVT_LEFT_DOWN, &SetDrawPane::mouse_down_event, this);
    Bind(wxEVT_LEFT_UP, &SetDrawPane::mouse_release_event, this);
    Bind(wxEVT_MOUSEWHEEL, &SetDrawPane::mouse_wheel_event, this);
    Bind(wxEVT_MOTION, &SetDrawPane::mouse_move_event, this);
  }

  void render(wxDC* dc, wxImage* image) {
    wxBitmap* bitmap = new wxBitmap(*image);
    dc->DrawBitmap(*bitmap, 0, 0, false);
  }

  virtual void paint_event(wxPaintEvent& event) = 0;

  void mouse_wheel_event(wxMouseEvent& event) {
    if (event.GetWheelAxis() == wxMOUSE_WHEEL_VERTICAL) {
      wxCoord width;
      wxCoord height;
      GetSize(&width, &height);
      wxCoord x = event.GetX();
      wxCoord y = event.GetY();

      double delta = (double) event.GetWheelRotation() / event.GetWheelDelta();
      /* Todo make scroll at cursor position */
      double origin_view_width = view_width;
      view_width *= pow(0.8, delta);

      double x_correction = +(origin_view_width - view_width) / width * (x - width / 2);
      double y_correction = -(origin_view_width - view_width) / width * (y - height / 2);

      view_center_x += x_correction;
      view_center_y += y_correction;
      tmp_center_x = view_center_x;
      tmp_center_y = view_center_y;

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

class JuliaDrawPane : public SetDrawPane {
public:
  double complex julia_seed;

  JuliaDrawPane(wxWindow* parent) : SetDrawPane(parent) {
    julia_seed = 0;
  }

  static void set_julia_seed(void* void_pane, double complex julia_seed) {
    JuliaDrawPane* pane = (JuliaDrawPane*) void_pane;
    pane->julia_seed = julia_seed;
    pane->Refresh();
  }

  void paint_event(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxCoord width;
    wxCoord height;
    dc.GetSize(&width, &height);
    width *= 2;
    height *= 2;
    unsigned char* data = (unsigned char*) malloc(3 * sizeof(char) * width * height);

    render_set_rgb(data, (unsigned int) width, (unsigned int) height, view_width, tmp_center_x, tmp_center_y, 100, false, julia_seed);

    /* wxImage will free data */
    wxImage image(width, height, data);
    image.Rescale(width / 2, height / 2);
    render(&dc, &image);
  }
};

class MandelbrotDrawPane : public SetDrawPane {
public:
  MandelbrotDrawPane(wxWindow* parent) : SetDrawPane(parent) {
    /* Nothing */
    update_function = NULL;
    user_data = NULL;
    Bind(wxEVT_LEFT_DCLICK, &MandelbrotDrawPane::mouse_left_double_click_event, this);
  }

  void mouse_left_double_click_event(wxMouseEvent& event) {
    if (update_function != NULL) {
      wxCoord width;
      wxCoord height;
      GetSize(&width, &height);

      double x = tmp_center_x + view_width / width * (event.GetX() - width / 2);
      double y = tmp_center_y - view_width / width * (event.GetY() - height / 2);

      update_function(user_data, x + I * y);
    }
  }

  void paint_event(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxCoord width;
    wxCoord height;
    dc.GetSize(&width, &height);
    width *= 2;
    height *= 2;
    unsigned char* data = (unsigned char*) malloc(3 * sizeof(char) * width * height);

    render_set_rgb(data, (unsigned int) width, (unsigned int) height, view_width, tmp_center_x, tmp_center_y, 100, true, 0);

    /* wxImage will free data */
    wxImage image(width, height, data);
    image.Rescale(width / 2, height / 2);
    render(&dc, &image);
  }

  void (*update_function) (void*, double complex);
  void* user_data;
  void register_update_event(void (*function) (void*, double complex), void* user_data) {
    this->update_function = function;
    this->user_data = user_data;
  }
};

class JuliaWxMainPanel : public wxPanel {
public:
  JuliaWxMainPanel(wxWindow* parent) : wxPanel(parent) {
    wxGridSizer* sizer = new wxGridSizer(1, 2, 0, 0);

    MandelbrotDrawPane* mandel_pane = new MandelbrotDrawPane(this);
    sizer->Add(mandel_pane, 1, wxEXPAND);

    JuliaDrawPane* julia_pane = new JuliaDrawPane(this);
    sizer->Add(julia_pane, 1, wxEXPAND);

    mandel_pane->register_update_event(&JuliaDrawPane::set_julia_seed, julia_pane);

    //sizer->SetMinSize(10, 10);
    SetSizerAndFit(sizer);
  }
};

class JuliaWxMainFrame : public wxFrame {
public:
  JuliaWxMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size) {
    JuliaWxMainPanel* panel = new JuliaWxMainPanel(this);
    panel->Show();
  }
};

class SetMapWxApp : public wxApp {
public:
  bool OnInit() {
    JuliaWxMainFrame* frame = new JuliaWxMainFrame(wxT("Julia/Mandelbrot Map"), wxDefaultPosition, wxDefaultSize);
    frame->Show(true);

    return true;
  }
};
wxIMPLEMENT_APP_NO_MAIN(SetMapWxApp);

void create_wx_window(void) {
  int argc = 0;
  char** argv = NULL;
  wxEntry(argc, argv);
}

int main(int argc, char** argv) {
  create_wx_window();
  return 0;
}
