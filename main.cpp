#include <wx/wx.h>
#include <wx/sizer.h>

class JuliaDrawPane : public wxPanel {
public:
  JuliaDrawPane(wxWindow* parent)
    : wxPanel(parent) {
    
  }
  void render(wxDC* dc, wxImage* image) {
    wxBitmap* bitmap = new wxBitmap(*image);
    dc->DrawBitmap(*bitmap, 0, 0, false);
  }

  void paintEvent(wxPaintEvent& event) {
    wxPaintDC dc(this);
    render(&dc, NULL);
  }

  /*
   void mouseMoved(wxMouseEvent& event);
   void mouseDown(wxMouseEvent& event);
   void mouseWheelMoved(wxMouseEvent& event);
   void mouseReleased(wxMouseEvent& event);
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
