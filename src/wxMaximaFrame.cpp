///
///  Copyright (C) 2004-2009 Andrej Vodopivec <andrejv@users.sourceforge.net>
///
///  This program is free software; you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
///  the Free Software Foundation; either version 2 of the License, or
///  (at your option) any later version.
///
///  This program is distributed in the hope that it will be useful,
///  but WITHOUT ANY WARRANTY; without even the implied warranty of
///  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
///  GNU General Public License for more details.
///
///
///  You should have received a copy of the GNU General Public License
///  along with this program; if not, write to the Free Software
///  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///

#include "wxMaximaFrame.h"

#include <wx/artprov.h>
#include <wx/config.h>
#include <wx/image.h>
#include <wx/filename.h>


wxMaximaFrame::wxMaximaFrame(wxWindow* parent, int id, const wxString& title,
                             const wxPoint& pos, const wxSize& size,
                             long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
  m_manager.SetManagedWindow(this);

//  m_consolePanel = new wxPanel(this, -1);
  m_simpPanel = new wxPanel(this, -1);
  m_trigPanel = new wxPanel(this, -1);
  m_solvePanel = new wxPanel(this, -1);
  m_calcPanel = new wxPanel(this, -1);
  m_plotPanel = new wxPanel(this, -1);

  button_2 = new wxButton(m_simpPanel, button_ratsimp, _("Simplify"));
  button_3 = new wxButton(m_simpPanel, button_radcan, _("Simplify (r)"));
  button_4 = new wxButton(m_simpPanel, button_factor, _("Factor"));
  button_5 = new wxButton(m_simpPanel, button_expand, _("Expand"));
  button_6 = new wxButton(m_trigPanel, button_trigsimp, _("Simplify"));
  button_7 = new wxButton(m_trigPanel, button_trigexpand, _("Expand"));
  button_8 = new wxButton(m_trigPanel, button_trigreduce, _("Reduce"));
  button_22 = new wxButton(m_trigPanel, button_trigrat, _("Cannonical"));
  button_9 = new wxButton(m_simpPanel, button_rectform, _("Rectform"));
//  button_10 = new wxButton(m_othrPanel, button_sum, _("Sum..."));
//  button_11 = new wxButton(m_othrPanel, button_product, _("Product..."));
  button_12 = new wxButton(m_solvePanel, button_solve, _("Solve..."));
  button_13 = new wxButton(m_solvePanel, button_solve_ode, _("Solve ODE..."));
  button_14 = new wxButton(m_calcPanel, button_diff, _("Diff..."));
  button_15 = new wxButton(m_calcPanel, button_integrate, _("Integrate..."));
  button_16 = new wxButton(m_calcPanel, button_limit, _("Limit..."));
  button_17 = new wxButton(m_calcPanel, button_taylor, _("Series..."));
  button_18 = new wxButton(m_simpPanel, button_subst, _("Subst..."));
  //button_19 = new wxButton(m_othrPanel, button_map, _("Map..."));
  button_20 = new wxButton(m_plotPanel, button_plot2, _("Plot 2D..."));
  button_21 = new wxButton(m_plotPanel, button_plot3, _("Plot 3D..."));

  // console
  m_console = new MathCtrl(this, -1, wxDefaultPosition, wxDefaultSize);

  // history
  m_history = new History(this, -1);

  SetupMenu();
#if defined (__WXMSW__) || defined (__WXGTK20__) || defined (__WXMAC__)
  SetupToolBar();
#endif

  frame_1_statusbar = CreateStatusBar(2);
  int widths[] =
    {
      -1, 300
    };
  SetStatusWidths(2, widths);

#if defined __WXMSW__
  wxAcceleratorEntry entries[0];
  entries[0].Set(wxACCEL_CTRL,  WXK_RETURN, menu_evaluate);
  wxAcceleratorTable accel(1, entries);
  SetAcceleratorTable(accel);
#endif

  set_properties();
  do_layout();

  m_console->SetFocus();
}

wxMaximaFrame::~wxMaximaFrame()
{
  wxString perspective = m_manager.SavePerspective();

  wxConfig::Get()->Write(wxT("AUI/perspective"), perspective);

  m_manager.UnInit();
}

void wxMaximaFrame::set_properties()
{
#if defined (__WXMSW__)
  SetIcon(wxICON(icon0));
#elif defined (__WXGTK__)
  wxString icon(wxT(PREFIX));
  icon += wxT("/share/wxMaxima/wxmaxima.png");
  SetIcon(wxIcon(icon, wxBITMAP_TYPE_PNG));
#endif
  SetTitle(wxString::Format(_("wxMaxima %s "), wxT(VERSION)) + _("[ unsaved ]"));

  m_console->SetBackgroundColour(wxColour(wxT("WHITE")));
  m_console->SetMinSize(wxSize(100, 100));
  frame_1_statusbar->SetStatusText(_("Welcome to wxMaxima"), 0);
}

void wxMaximaFrame::do_layout()
{
  wxGridSizer* simpGrid = new wxGridSizer(2, 3);
  wxBoxSizer* simpBox = new wxBoxSizer(wxVERTICAL);

  wxGridSizer* trigGrid = new wxGridSizer(2, 2);
  wxBoxSizer* trigBox = new wxBoxSizer(wxVERTICAL);

  wxGridSizer* solveGrid = new wxGridSizer(2, 1);
  wxBoxSizer* solveBox = new wxBoxSizer(wxVERTICAL);

  wxGridSizer* calcGrid = new wxGridSizer(2, 2);
  wxBoxSizer* calcBox = new wxBoxSizer(wxVERTICAL);

  wxGridSizer* plotGrid = new wxGridSizer(2, 1);
  wxBoxSizer* plotBox = new wxBoxSizer(wxVERTICAL);

  int style = wxALL | wxEXPAND;
  simpGrid->Add(button_2, 0, style, 0);
  simpGrid->Add(button_3, 0, style, 0);
  simpGrid->Add(button_4, 0, style, 0);
  simpGrid->Add(button_5, 0, style, 0);
  trigGrid->Add(button_6, 0, style, 0);
  trigGrid->Add(button_7, 0, style, 0);
  trigGrid->Add(button_8, 0, style, 0);
  trigGrid->Add(button_22, 0, style, 0);
  simpGrid->Add(button_9, 0, style, 0);
//  grid_sizer_2->Add(button_10, 0, style, 0);
//  grid_sizer_2->Add(button_11, 0, style, 0);
  solveGrid->Add(button_12, 0, style, 0);
  solveGrid->Add(button_13, 0, style, 0);
  calcGrid->Add(button_14, 0, style, 0);
  calcGrid->Add(button_15, 0, style, 0);
  calcGrid->Add(button_16, 0, style, 0);
  calcGrid->Add(button_17, 0, style, 0);
  simpGrid->Add(button_18, 0, style, 0);
//  grid_sizer_2->Add(button_19, 0, style, 0);
  plotGrid->Add(button_20, 0, style, 0);
  plotGrid->Add(button_21, 0, style, 0);

  m_manager.AddPane(m_console,
      wxAuiPaneInfo().Name(wxT("console")).
                      Center().
                      CloseButton(false).
                      CaptionVisible(false).
                      PaneBorder(false));

#define ADD_PANE(grid, box, panel, name, caption)  \
  box->Add(grid, 0, wxEXPAND | wxALL, 3);          \
  panel->SetSizer(box);                            \
  box->Fit(panel);                                 \
  box->SetSizeHints(panel);                        \
  m_manager.AddPane(panel,                         \
      wxAuiPaneInfo().Name(name).                  \
                      Caption(caption).            \
                      Show(false).                 \
                      TopDockable(false).          \
                      BottomDockable(false).       \
                      PaneBorder(true).            \
                      Fixed().                     \
                      Left());

  m_manager.AddPane(m_history,
      wxAuiPaneInfo().Name(wxT("history")).
                      Caption(_("History")).
                      Show(false).
                      TopDockable(false).
                      BottomDockable(false).
                      PaneBorder(true).
                      Right());


  ADD_PANE(simpGrid, simpBox, m_simpPanel, wxT("simplify"), _("Simplify"));
  ADD_PANE(calcGrid, calcBox, m_calcPanel, wxT("calc"), _("Calculus"));
  ADD_PANE(solveGrid, solveBox, m_solvePanel, wxT("solve"), _("Solve"));
  ADD_PANE(trigGrid, trigBox, m_trigPanel, wxT("trig"), _("Trigonometry"));
  ADD_PANE(plotGrid, plotBox, m_plotPanel, wxT("plot"), _("Plot"));

#undef ADD_PANE

  wxString perspective;
  wxConfigBase *config = wxConfig::Get();

  if (config->Read(wxT("AUI/perspective"), &perspective))
    m_manager.LoadPerspective(perspective);
  else
    m_manager.Update();
}

void wxMaximaFrame::SetupMenu()
{
  frame_1_menubar = new wxMenuBar();

#if defined __WXGTK20__
  wxMenuItem *tmp_menu_item;
#define APPEND_MENU_ITEM(menu, id, label, help, stock)                         \
  tmp_menu_item = new wxMenuItem((menu), (id), (label), (help), wxITEM_NORMAL); \
  tmp_menu_item->SetBitmap(wxArtProvider::GetBitmap((stock), wxART_MENU));      \
  (menu)->Append(tmp_menu_item);
#else
#define APPEND_MENU_ITEM(menu, id, label, help, stock) \
  (menu)->Append((id), (label), (help), wxITEM_NORMAL);
#endif

  // File menu
  wxMenu* wxglade_tmp_menu_1 = new wxMenu;
#if defined __WXMAC__
  wxglade_tmp_menu_1->Append(mac_newId, _("&New Window\tCtrl-N"),
			     _("Open a new window"));
#endif
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, menu_open_id, _("&Open...\tCtrl-O"),
                   _("Open a document"), wxT("gtk-open"));
  m_recentDocumentsMenu = new wxMenu();
  wxglade_tmp_menu_1->Append(menu_recent_documents, _("Open Recent"), m_recentDocumentsMenu);
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, menu_save_id, _("&Save\tCtrl-S"),
                   _("Save document"), wxT("gtk-save"));
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, menu_save_as_id, _("Save As...\tShift-Ctrl-S"),
                   _("Save document as ..."), wxT("gtk-save"));
  wxglade_tmp_menu_1->Append(menu_load_id, _("&Load Package...\tCtrl-L"),
                             _("Load a Maxima package file"), wxITEM_NORMAL);
  wxglade_tmp_menu_1->Append(menu_batch_id, _("&Batch File...\tCtrl-B"),
                             _("Load a Maxima file using the batch command"), wxITEM_NORMAL);
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, menu_export_html, _("&Export..."),
                   _("Export document to a HTML or LaTeX file"), wxT("stock_export"));
#if WXM_PRINT
  wxglade_tmp_menu_1->AppendSeparator();
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, wxID_PRINT, _("&Print...\tCtrl-P"),
                   _("Print document"), wxT("gtk-print"));
#endif

  wxglade_tmp_menu_1->AppendSeparator();
  APPEND_MENU_ITEM(wxglade_tmp_menu_1, wxID_EXIT, _("E&xit\tCtrl-Q"),
                   _("Exit wxMaxima"), wxT("gtk-quit"));
  frame_1_menubar->Append(wxglade_tmp_menu_1, _("&File"));

  // Edit menu
  wxMenu* wxglade_tmp_menu_2 = new wxMenu;
  wxglade_tmp_menu_2->Append(menu_undo, _("Undo\tCtrl-Z"),
                             _("Undo last change"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->AppendSeparator();
  wxglade_tmp_menu_2->Append(menu_cut, _("Cut\tCtrl-X"),
                             _("Cut selection"),
                             wxITEM_NORMAL);
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_copy_from_console, _("&Copy\tCtrl-C"),
                   _("Copy selection"), wxT("gtk-copy"));
  wxglade_tmp_menu_2->Append(menu_copy_tex_from_console, _("Copy LaTeX"),
                                  _("Copy selection from document in LaTeX format"),
                                  wxITEM_NORMAL);
#if defined __WXMSW__ || defined __WXMAC__
  wxglade_tmp_menu_2->Append(menu_copy_as_bitmap, _("Copy as Image"),
                             _("Copy selection from document as an image"),
                             wxITEM_NORMAL);
#endif
  wxglade_tmp_menu_2->Append(menu_paste, _("Paste\tCtrl-V"),
                             _("Paste text from clipboard"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->AppendSeparator();
  wxglade_tmp_menu_2->Append(menu_select_all, _("Select All\tCtrl-A"),
                             _("Select all"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_copy_to_file, _("Save Selection to Image..."),
                             _("Save selection from document to an image file"),
                             wxITEM_NORMAL);
  wxMenu* wxglade_tmp_menu_2_sub1 = new wxMenu;
  wxglade_tmp_menu_2_sub1->Append(menu_cut_input_from_console, _("Cut Cell(s)\tCtrl-Shift-X"),
                                  _("Cut selected cell(s)"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_2_sub1->Append(menu_copy_input_from_console, _("Copy Cell(s)\tCtrl-Shift-C"),
                                  _("Copy selected cell(s)"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_2_sub1->Append(menu_paste_input, _("Paste Cell(s)\tCtrl-Shift-V"),
                             _("Paste cell(s) to document"),
                             wxITEM_NORMAL);
  APPEND_MENU_ITEM(wxglade_tmp_menu_2_sub1, menu_delete_selection,
                   _("&Delete Cell(s)"),
                   _("Delete selected cell(s)"), wxT("gtk-delete"));
  wxglade_tmp_menu_2_sub1->AppendSeparator();
  wxglade_tmp_menu_2_sub1->Append(menu_evaluate, _("Evaluate Cell(s)"),
                             _("Evaluate active or selected cell(s)"), wxITEM_NORMAL);
  wxglade_tmp_menu_2_sub1->Append(menu_evaluate_all, _("Evaluate All Cells\tCtrl-R"),
                               _("Evaluate all cells in the document"), wxITEM_NORMAL);
  wxglade_tmp_menu_2_sub1->AppendSeparator();
  wxglade_tmp_menu_2_sub1->Append(menu_insert_input, _("New Input &Cell\tF5"),
                             _("Insert a new input cell"));
  wxglade_tmp_menu_2_sub1->Append(menu_add_comment, _("New &Text Cell\tF6"),
                             _("Insert a new text cell"));
  wxglade_tmp_menu_2_sub1->Append(menu_add_subsection, _("New S&ubsection Cell\tF7"),
                             _("Insert a new subsection cell"));
  wxglade_tmp_menu_2_sub1->Append(menu_add_section, _("New &Section Cell\tF8"),
                             _("Insert a new section cell"));
  wxglade_tmp_menu_2_sub1->Append(menu_add_title, _("New T&itle Cell\tF9"),
                             _("Insert a new title cell"));
  wxglade_tmp_menu_2->AppendSeparator();

  wxglade_tmp_menu_2->Append(wxNewId(), _("Cell"), wxglade_tmp_menu_2_sub1, _("Input"));
  wxglade_tmp_menu_2->Append(menu_remove_output, _("Remove All Outputs"),
                            _("Remove output from input cells"), wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_insert_image, _("Insert Image..."),
                              _("Insert image"), wxITEM_NORMAL);
  wxglade_tmp_menu_2->AppendSeparator();
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_zoom_in, _("Zoom &In\tAlt-I"),
                   _("Zoom in 10%"), wxT("gtk-zoom-in"));
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_zoom_out, _("Zoom Ou&t\tAlt-O"),
                   _("Zoom out 10%"), wxT("gtk-zoom-out"));
  // zoom submenu
  wxMenu* edit_zoom_sub = new wxMenu;
  edit_zoom_sub->Append(menu_zoom_80,  wxT("80%"),  _("Set zoom to 80%"),  wxITEM_NORMAL);
  edit_zoom_sub->Append(menu_zoom_100, wxT("100%"), _("Set zoom to 100%"), wxITEM_NORMAL);
  edit_zoom_sub->Append(menu_zoom_120, wxT("120%"), _("Set zoom to 120%"), wxITEM_NORMAL);
  edit_zoom_sub->Append(menu_zoom_150, wxT("150%"), _("Set zoom to 150%"), wxITEM_NORMAL);
  edit_zoom_sub->Append(menu_zoom_200, wxT("200%"), _("Set zoom to 200%"), wxITEM_NORMAL);
  edit_zoom_sub->Append(menu_zoom_300, wxT("300%"), _("Set zoom to 300%"), wxITEM_NORMAL);

  wxglade_tmp_menu_2->Append(wxNewId(), _("Set Zoom"), edit_zoom_sub, _("Set Zoom"));


  wxglade_tmp_menu_2->Append(menu_fullscreen, _("Full Screen\tAlt-Enter"),
                             _("Toggle full screen editing"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->AppendSeparator();
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, wxID_PREFERENCES, _("C&onfigure"),
                   _("Configure wxMaxima"), wxT("gtk-preferences"));
  frame_1_menubar->Append(wxglade_tmp_menu_2, _("&Edit"));

  // Maxima menu
  wxglade_tmp_menu_2 = new wxMenu;

  // palettes
  wxMenu *wxglade_tmp_menu_2_sub2 = new wxMenu;
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_simplify, _("Simplify palette"));
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_trig, _("Trigonometry palette"));
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_solve, _("Solve palette"));
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_calc, _("Calculus palette"));
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_plot, _("Plot palette"));
  wxglade_tmp_menu_2_sub2->AppendCheckItem(menu_palette_history, _("History"));
  wxglade_tmp_menu_2->Append(wxNewId(), _("Palettes"), wxglade_tmp_menu_2_sub2);

  wxglade_tmp_menu_2->AppendSeparator();


#if defined (__WXMAC__)
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_interrupt_id,
                   _("&Interrupt\tCtrl-."), // command-. interrupts (mac standard)
                   _("Interrupt current computation"), wxT("gtk-stop"));
#else
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_interrupt_id,
                   _("&Interrupt\tCtrl-G"),
                   _("Interrupt current computation"), wxT("gtk-stop"));
#endif
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_restart_id,
                   _("&Restart Maxima"), _("Restart Maxima"), wxT("gtk-refresh"));
  wxglade_tmp_menu_2->Append(menu_soft_restart, _("&Clear Memory"),
                             _("Delete all values from memory"), wxITEM_NORMAL);
  APPEND_MENU_ITEM(wxglade_tmp_menu_2, menu_add_path, _("Add to &Path..."),
                   _("Add a directory to search path"), wxT("gtk-add"));

  wxglade_tmp_menu_2->AppendSeparator();
  wxglade_tmp_menu_2->Append(menu_functions, _("Show &Functions"),
                             _("Show defined functions"), wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_fun_def, _("Show &Definition..."),
                             _("Show definition of a function"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_variables, _("Show &Variables"),
                             _("Show defined variables"), wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_clear_fun, _("Delete F&unction..."),
                             _("Delete a function"), wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_clear_var, _("Delete V&ariable..."),
                             _("Delete a variable"), wxITEM_NORMAL);

  wxglade_tmp_menu_2->AppendSeparator();
  wxglade_tmp_menu_2->Append(menu_time, _("Toggle &Time Display"),
                             _("Display time used for evaluation"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_display, _("Change &2d Display"),
                             _("Change the 2d display algorithm used to display math output"),
			     wxITEM_NORMAL);
  wxglade_tmp_menu_2->Append(menu_texform, _("Display Te&X Form"),
                             _("Display last result in TeX form"), wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_2, _("&Maxima"));

  // Equations menu
  wxMenu* wxglade_tmp_menu_3 = new wxMenu;
  wxglade_tmp_menu_3->Append(menu_solve, _("&Solve..."),
                             _("Solve equation(s)"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_solve_num, _("&Find Root..."),
                             _("Find a root of an equation on an interval"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_allroots, _("Roots of &Polynomial"),
                             _("Find all roots of a polynomial"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_realroots, _("&Roots of Polynomial (Real)"),
                             _("Find real roots of a polynomial"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_solve_lin, _("Solve &Linear System..."),
                             _("Solve linear system of equations"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_solve_algsys, _("Solve &Algebraic System..."),
                             _("Solve algebraic system of equations"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_eliminate, _("&Eliminate Variable..."),
                             _("Eliminate a variable from a system "
                               "of equations"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->AppendSeparator();
  wxglade_tmp_menu_3->Append(menu_solve_ode, _("Solve &ODE..."),
                             _("Solve ordinary differential equation "
                               "of maximum degree 2"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_ivp_1, _("Initial Value Problem (&1)..."),
                             _("Solve initial value problem for first"
                               " degree ODE"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_ivp_2, _("Initial Value Problem (&2)..."),
                             _("Solve initial value problem for second "
                               "degree ODE"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_bvp, _("&Boundary Value Problem..."),
                             _("Solve boundary value problem for second "
                               "degree ODE"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->AppendSeparator();
  wxglade_tmp_menu_3->Append(menu_solve_de, _("Solve ODE with Lapla&ce..."),
                             _("Solve ordinary differential equations "
                               "with Laplace transformation"), wxITEM_NORMAL);
  wxglade_tmp_menu_3->Append(menu_atvalue, _("A&t Value..."),
                             _("Setup atvalues for solving ODE with "
                               "Laplace transformation"), wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_3, _("E&quations"));

  // Algebra menu
  wxMenu* wxglade_tmp_menu_4 = new wxMenu;
  wxglade_tmp_menu_4->Append(menu_gen_mat, _("&Generate Matrix..."),
                             _("Generate a matrix from a 2-dimensional array"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_enter_mat, _("&Enter Matrix..."),
                             _("Enter a matrix"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_invert_mat, _("&Invert Matrix"),
                             _("Compute the inverse of a matrix"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_cpoly, _("&Characteristic Polynomial..."),
                             _("Compute the characteristic polynomial "
                               "of a matrix"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_determinant, _("&Determinant"),
                             _("Compute the determinant of a matrix"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_eigen, _("Eigen&values"),
                             _("Find eigenvalues of a matrix"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_eigvect, _("Eige&nvectors"),
                             _("Find eigenvectors of a matrix"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_adjoint_mat, _("Ad&joint Matrix"),
                             _("Compute the adjoint maxrix"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_transpose, _("&Transpose Matrix"),
                             _("Transpose a matrix"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->AppendSeparator();
  wxglade_tmp_menu_4->Append(menu_make_list, _("Make &List..."),
                             _("Make list from expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_apply, _("&Apply to List..."),
                             _("Apply function to a list"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_map, _("&Map to List..."),
                             _("Map function to a list"), wxITEM_NORMAL);
  wxglade_tmp_menu_4->Append(menu_map_mat, _("Ma&p to Matrix..."),
                             _("Map function to a matrix"), wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_4, _("&Algebra"));

  // Calculus menu
  wxMenu* wxglade_tmp_menu_6 = new wxMenu;
  wxglade_tmp_menu_6->Append(menu_integrate, _("&Integrate..."),
                             _("Integrate expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_risch, _("Risch Integration..."),
                             _("Integrate expression with Risch algorithm"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_change_var, _("C&hange Variable..."),
                             _("Change variable in integral or sum"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_diff, _("&Differentiate..."),
                             _("Differentiate expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_limit, _("Find &Limit..."),
                             _("Find a limit of an expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_series, _("Get &Series..."),
                             _("Get the Taylor or power series of expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_pade, _("P&ade Approximation..."),
                             _("Pade approximation of a Taylor series"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_sum, _("Calculate Su&m..."),
                             _("Calculate sums"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_product, _("Calculate &Product..."),
                             _("Calculate products"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_laplace, _("Laplace &Transform..."),
                             _("Get Laplace transformation of an expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_ilt, _("Inverse Laplace T&ransform..."),
                             _("Get inverse Laplace transformation of an expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_gcd, _("&Greatest Common Divisor..."),
                             _("Compute the greatest common divisor"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_lcm, _("Least Common Multiple..."),
                             _("Compute the least common multiple "
                               "(do load(functs) before using)"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_divide, _("Di&vide Polynomials..."),
                             _("Divide numbers or polynomials"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_partfrac, _("Partial &Fractions..."),
                             _("Decompose rational function to partial fractions"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_continued_fraction, _("&Continued Fraction"),
                             _("Compute continued fraction of a value"),
                             wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_6, _("&Calculus"));

  // Simplify menu
  wxMenu* wxglade_tmp_menu_5 = new wxMenu;
  wxglade_tmp_menu_5->Append(menu_ratsimp, _("&Simplify Expression"),
                             _("Simplify rational expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_radsimp, _("Simplify &Radicals"),
                             _("Simplify expression containing radicals"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_factor, _("&Factor Expression"),
                             _("Factor an expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_gfactor, _("Factor Complex"),
                             _("Factor an expression in Gaussian numbers"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_expand, _("&Expand Expression"),
                             _("Expand an expression"), wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_logexpand, _("Expand Logarithms"),
                             _("Convert logarithm of product to sum of logarithms"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_logcontract, _("Contract Logarithms"),
                             _("Convert sum of logarithms to logarithm of product"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->AppendSeparator();
  // Factorials and gamma
  wxMenu* wxglade_tmp_menu_5_sub1 = new wxMenu;
  wxglade_tmp_menu_5_sub1->Append(menu_to_fact, _("Convert to &Factorials"),
                                  _("Convert binomials, beta and gamma function to factorials"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub1->Append(menu_to_gamma, _("Convert to &Gamma"),
                                  _("Convert binomials, factorials and beta function to gamma function"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub1->Append(menu_factsimp, _("&Simplify Factorials"),
                                  _("Simplify an expression containing factorials"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub1->Append(menu_factcomb, _("&Combine Factorials"),
                                  _("Combine factorials in an expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(wxNewId(), _("Factorials and &Gamma"),
                             wxglade_tmp_menu_5_sub1,
                             _("Functions for simplifying factorials and gamma function"));
  // Trigonometric
  wxMenu* wxglade_tmp_menu_5_sub2 = new wxMenu;
  wxglade_tmp_menu_5_sub2->Append(menu_trigsimp, _("&Simplify Trigonometric"),
                                  _("Simplify trigonometric expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub2->Append(menu_trigreduce, _("&Reduce Trigonometric"),
                                  _("Reduce trigonometric expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub2->Append(menu_trigexpand, _("&Expand Trigonometric"),
                                  _("Expand trigonometric expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub2->Append(menu_trigrat, _("&Canonical Form"),
                                  _("Convert trigonometric expression to canonical quasilinear form"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(wxNewId(), _("&Trigonometric Simplification"),
                             wxglade_tmp_menu_5_sub2,
                             _("Functions for simplifying trigonometric expressions"));
  // Complex
  wxMenu* wxglade_tmp_menu_5_sub3 = new wxMenu;
  wxglade_tmp_menu_5_sub3->Append(menu_rectform, _("Convert to &Rectform"),
                                  _("Convert complex expression to rect form"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub3->Append(menu_polarform, _("Convert to &Polarform"),
                                  _("Convert complex expression to polar form"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub3->Append(menu_realpart, _("Get Real P&art"),
                                  _("Get the real part of complex expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub3->Append(menu_imagpart, _("Get &Imaginary Part"),
                                  _("Get the imaginary part of complex expression"),
                                  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub3->Append(menu_demoivre, _("&Demoivre"),
                                  _("Convert exponential function of imaginary argument to trigonometric form"),
				  wxITEM_NORMAL);
  wxglade_tmp_menu_5_sub3->Append(menu_exponentialize, _("&Exponentialize"),
                                  _("Conver trigonometric functions to exponential form"),
				  wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(wxNewId(), _("&Complex Simplification"),
                             wxglade_tmp_menu_5_sub3,
                             _("Functions for complex simplification"));
  wxglade_tmp_menu_5->AppendSeparator();
  wxglade_tmp_menu_5->Append(menu_subst, _("Substitute..."),
                             _("Make substitution in expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_nouns, _("Evaluate &Noun Forms"),
                             _("Evaluate all noun forms in expression"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_talg, _("Toggle &Algebraic Flag"),
                             _("Toggle algebraic flag"), wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_tellrat, _("Add Algebraic E&quality..."),
                             _("Add equality to the rational simplifier"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_5->Append(menu_modulus, _("&Modulus Computation..."),
                             _("Setup modulus computation"), wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_5, _("&Simplify"));

  // Plot menu
  wxglade_tmp_menu_6 = new wxMenu;
  wxglade_tmp_menu_6->Append(gp_plot2, _("Plot &2d..."),
                             _("Plot in 2 dimensions"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(gp_plot3, _("Plot &3d..."),
                             _("Plot in 3 dimensions"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_plot_format, _("Plot &Format..."),
                             _("Set plot format"), wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_6, _("&Plot"));

  // Numeric menu
  wxglade_tmp_menu_6 = new wxMenu;
  wxglade_tmp_menu_6->Append(menu_num_out, _("Toggle &Numeric Output"),
                             _("Toggle numeric output"), wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_to_float, _("To &Float\tCtrl-F"),
                             _("Calculate float value of the last result"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_to_bfloat, _("To &Bigfloat"),
                             _("Calculate bigfloat value of the last result"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_6->Append(menu_set_precision, _("Set &Precision..."),
                             _("Set bigfloat precision"),
                             wxITEM_NORMAL);
  frame_1_menubar->Append(wxglade_tmp_menu_6, _("&Numeric"));

  // Help menu
  wxMenu* wxglade_tmp_menu_7 = new wxMenu;
  APPEND_MENU_ITEM(wxglade_tmp_menu_7, wxID_HELP, _("Maxima &Help\tF1"),
                   _("Show Maxima help"), wxT("gtk-help"));
  wxglade_tmp_menu_7->Append(menu_example, _("&Example..."),
                             _("Show an example of usage"),
                             wxITEM_NORMAL);
  wxglade_tmp_menu_7->Append(menu_apropos, _("&Apropos..."),
                             _("Show commands similar to"),
                             wxITEM_NORMAL);
  APPEND_MENU_ITEM(wxglade_tmp_menu_7, menu_show_tip, _("Show &Tips..."),
                   _("Show a tip"), wxART_TIP);
  wxglade_tmp_menu_7->AppendSeparator();
  wxglade_tmp_menu_7->Append(menu_build_info, _("Build &Info"),
                             _("Info about Maxima build"), wxITEM_NORMAL);
  wxglade_tmp_menu_7->Append(menu_bug_report, _("&Bug Report"),
                             _("Report bug"), wxITEM_NORMAL);
#ifndef __WXMAC__
  wxglade_tmp_menu_7->AppendSeparator();
#endif
  APPEND_MENU_ITEM(wxglade_tmp_menu_7, wxID_ABOUT,
#ifndef __WXMAC__
          _("About"),
#else
          _("About wxMaxima"),
#endif
          _("About wxMaxima"), wxT("stock_about"));
  frame_1_menubar->Append(wxglade_tmp_menu_7, _("&Help"));

  SetMenuBar(frame_1_menubar);

#undef APPEND_MENU_ITEM

}

#if defined (__WXMSW__) || defined (__WXMAC__)

#if defined (__WXMSW__)
#define IMAGE(img) wxImage(wxT("art/toolbar/") wxT(img))
#else
#define IMAGE(img) wxImage(wxT("wxMaxima.app/Contents/Resources/toolbar/") wxT(img))
#endif

void wxMaximaFrame::SetupToolBar()
{
  wxToolBar* frame_1_toolbar = CreateToolBar();

  frame_1_toolbar->SetToolBitmapSize(wxSize(22, 22));

  frame_1_toolbar->AddTool(tb_open, _("Open"),
                           IMAGE("open.png"),
			                     _("Open document"));
  frame_1_toolbar->AddTool(tb_save, _("Save"),
                           IMAGE("save.png"),
                           _("Save document"));
  frame_1_toolbar->AddSeparator();
#if WXM_PRINT
  frame_1_toolbar->AddTool(tb_print, _("Print"),
                           IMAGE("print.png"),
                           _("Print document"));
#endif
  frame_1_toolbar->AddTool(tb_pref, _("Options"),
                           IMAGE("configure.png"),
                           _("Configure wxMaxima"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_copy, _("Copy"),
                           IMAGE("copy.png"),
                           _("Copy selection"));
  frame_1_toolbar->AddTool(tb_delete, _("Delete"),
                           IMAGE("cut.png"),
                           _("Delete selection"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_insert_text, _("Insert text"),
                           IMAGE("text.png"),
                           _("Insert text"));
  frame_1_toolbar->AddTool(tb_insert_input, _("Insert input group"),
                           IMAGE("input.png"),
                           _("Insert input cell"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_interrupt, _("Interrupt"),
                           IMAGE("stop.png"),
                           _("Interrupt current computation"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_animation_start, _("Start animation"),
                           IMAGE("playback-start.png"),
                           _("Start animation"));
  frame_1_toolbar->AddTool(tb_animation_stop, _("Stop animation"),
			   IMAGE("playback-stop.png"),
			   _("Stop animation"));
  m_plotSlider = new wxSlider(frame_1_toolbar, plot_slider_id, 0, 0, 10,
			      wxDefaultPosition, wxDefaultSize,
			      wxSL_HORIZONTAL | !wxSL_AUTOTICKS);
  frame_1_toolbar->AddControl(m_plotSlider);
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_help, _("Help"),
                           IMAGE("help.png"),
                           _("Show Maxima help"));
  frame_1_toolbar->Realize();
  SetToolBar(frame_1_toolbar);
}

#elif defined (__WXGTK20__)

void wxMaximaFrame::SetupToolBar()
{
  wxToolBar* frame_1_toolbar = CreateToolBar();

  frame_1_toolbar->AddTool(tb_open, _("Open"),
                           wxArtProvider::GetBitmap(wxT("gtk-open"),
                                                    wxART_TOOLBAR),
                           _("Open document"));
  frame_1_toolbar->AddTool(tb_save, _("Save"),
                           wxArtProvider::GetBitmap(wxT("gtk-save"),
                                                    wxART_TOOLBAR),
                           _("Save document"));
  frame_1_toolbar->AddSeparator();
#if WXM_PRINT
  frame_1_toolbar->AddTool(tb_print, _("Print"),
                           wxArtProvider::GetBitmap(wxT("gtk-print"),
                                                    wxART_TOOLBAR),
                           _("Print document"));
#endif
  frame_1_toolbar->AddTool(tb_pref, _("Options"),
                           wxArtProvider::GetBitmap(wxT("gtk-preferences"),
                                                    wxART_TOOLBAR),
                           _("Configure wxMaxima"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_copy, _("Copy"),
                           wxArtProvider::GetBitmap(wxT("gtk-copy"),
                                                    wxART_TOOLBAR),
                           _("Copy selection"));
  frame_1_toolbar->AddTool(tb_delete, _("Delete"),
                           wxArtProvider::GetBitmap(wxT("gtk-cut"),
                                                    wxART_TOOLBAR),
                           _("Delete selection"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_insert_text, _("Insert text"),
                           wxArtProvider::GetBitmap(wxART_NORMAL_FILE,
                                                    wxART_TOOLBAR),
                           _("Insert text"));
  frame_1_toolbar->AddTool(tb_insert_input, _("Insert input cell"),
                           wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE,
                                                    wxART_TOOLBAR),
                           _("Insert input cell"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_interrupt, _("Interrupt"),
                           wxArtProvider::GetBitmap(wxT("gtk-stop"),
                                                    wxART_TOOLBAR),
                           _("Interrupt current computation"));
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_animation_start, _("Animation"),
			   wxArtProvider::GetBitmap(wxT("media-playback-start"),
						    wxART_TOOLBAR));
  frame_1_toolbar->AddTool(tb_animation_stop, _("Stop animation"),
			   wxArtProvider::GetBitmap(wxT("media-playback-stop"),
						    wxART_TOOLBAR));
  m_plotSlider = new wxSlider(frame_1_toolbar, plot_slider_id, 0, 0, 10,
			      wxDefaultPosition, wxSize(200, -1),
			      wxSL_HORIZONTAL | !wxSL_AUTOTICKS);
  frame_1_toolbar->AddControl(m_plotSlider);
  frame_1_toolbar->AddSeparator();
  frame_1_toolbar->AddTool(tb_help, _("Help"),
                           wxArtProvider::GetBitmap(wxT("gtk-help"),
                                                    wxART_TOOLBAR),
                           _("Show Maxima help"));
  frame_1_toolbar->Realize();
  SetToolBar(frame_1_toolbar);
}

#endif

void wxMaximaFrame::LoadRecentDocuments()
{
  wxConfigBase *config = wxConfig::Get();
  m_recentDocuments.Clear();

  for (int i=0; i<10; i++)
  {
    wxString recent = wxString::Format(wxT("RecentDocuments/document_%d"), i);
    wxString file;
    if (config->Read(recent, &file))
      m_recentDocuments.Add(file);
  }
}

void wxMaximaFrame::SaveRecentDocuments()
{
  wxConfigBase *config = wxConfig::Get();

  // Delete previous recent documents
  for (int i=0; i<10; i++) {
    wxString recent = wxString::Format(wxT("RecentDocuments/document_%d"), i);
    config->DeleteEntry(recent);
  }

  // Save new recent documents
  for (int i=0; i<m_recentDocuments.Count(); i++) {
    wxString recent = wxString::Format(wxT("RecentDocuments/document_%d"), i);
    config->Write(recent, m_recentDocuments[i]);
  }
}

void wxMaximaFrame::UpdateRecentDocuments()
{
  wxMenu* fileMenu = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(_("&File")));

  for (int i=menu_recent_document_0; i<= menu_recent_document_9; i++)
  {
    wxMenuItem *item = m_recentDocumentsMenu->Remove(i);
    delete item;
    item = NULL;

    if (i-menu_recent_document_0 < m_recentDocuments.Count())
    {
      wxFileName filename(m_recentDocuments[i - menu_recent_document_0]);
      wxString path(filename.GetPath()), fullname(filename.GetFullName());
      wxString label(fullname + wxT("   [ ") + path + wxT(" ]"));

      m_recentDocumentsMenu->Append(i, label);
    }
  }

  SaveRecentDocuments();
}

void wxMaximaFrame::AddRecentDocument(wxString file)
{
  m_recentDocuments.Remove(file);
  m_recentDocuments.Insert(file, 0);

  UpdateRecentDocuments();
}

void wxMaximaFrame::RemoveRecentDocument(wxString file)
{
  m_recentDocuments.Remove(file);

  UpdateRecentDocuments();
}

bool wxMaximaFrame::IsPaletteDisplayed(int id)
{
  bool displayed = false;

  switch (id) {
    case menu_palette_simplify:
      displayed = m_manager.GetPane(wxT("simplify")).IsShown();
      break;
    case menu_palette_trig:
      displayed = m_manager.GetPane(wxT("trig")).IsShown();
      break;
    case menu_palette_calc:
      displayed = m_manager.GetPane(wxT("calc")).IsShown();
      break;
    case menu_palette_solve:
      displayed = m_manager.GetPane(wxT("solve")).IsShown();
      break;
    case menu_palette_plot:
      displayed = m_manager.GetPane(wxT("plot")).IsShown();
      break;
    case menu_palette_history:
      displayed = m_manager.GetPane(wxT("history")).IsShown();
      break;
  }

  return displayed;
}

void wxMaximaFrame::ShowPalette(int id, bool show)
{
  switch (id) {
    case menu_palette_simplify:
      m_manager.GetPane(wxT("simplify")).Show(show);
      break;
    case menu_palette_trig:
       m_manager.GetPane(wxT("trig")).Show(show);
       break;
    case menu_palette_solve:
       m_manager.GetPane(wxT("solve")).Show(show);
       break;
    case menu_palette_calc:
       m_manager.GetPane(wxT("calc")).Show(show);
       break;
    case menu_palette_plot:
      m_manager.GetPane(wxT("plot")).Show(show);
      break;
    case menu_palette_history:
      m_manager.GetPane(wxT("history")).Show(show);
      break;
  }

  m_manager.Update();
}
