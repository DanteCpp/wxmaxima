// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2008-2009 Ziga Lenarcic <zigalenarcic@users.sourceforge.net>
//            (C) 2012-2013 Doug Ilijev <doug.ilijev@gmail.com>
//            (C) 2015-2018 Gunter Königsmann <wxMaxima@physikbuch.de>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

/*! \file
  This file defines the class ToolBar that represents wxMaxima's main tool bar.
 */

#include "ToolBar.h"
#include "../art/toolbar/images.h"
#include <wx/mstream.h>
#include <wx/wfstream.h>
#include "Dirstructure.h"
#include "GroupCell.h"
#include <wx/artprov.h>
#include <wx/filename.h>
#include "invalidImage.h"

#define TOOLBAR_ICON_SCALE (0.35)

#define ABS(val) ((val) >= 0 ? (val) : -(val))
#define MAX(a, b) ((a)>(b) ? (a) : (b))
#define MIN(a, b) ((a)>(b) ? (b) : (a))

wxImage ToolBar::GetImage(wxString name,
                          unsigned char *data_128, size_t len_128,
                          unsigned char *data_192, size_t len_192)
{
  double targetSize = wxGetDisplayPPI().x * TOOLBAR_ICON_SCALE;
  int prescale;

  int sizeA = 128 << 4;
  while(sizeA * 3 / 2 > targetSize && sizeA >= 32) {
    sizeA >>= 1;
  };

  int sizeB = 192 << 4;
  while(sizeB * 4 / 3 > targetSize && sizeB >= 32) {
    sizeB >>= 1;
  }

  if(ABS(targetSize - sizeA) < ABS(targetSize - sizeB)) {
    targetSize = sizeA;
    prescale = 128;
  } else {
    targetSize = sizeB;
    prescale = 192;
  }

  wxBitmap bmp = wxArtProvider::GetBitmap(name, wxART_TOOLBAR, wxSize(targetSize, targetSize));
  wxImage img;

  if(bmp.IsOk()) {
    img = bmp.ConvertToImage();
  }
  if(!img.IsOk()) {
    void *data;
    size_t len;
    if(prescale == 128)
    {
      data = (void *)data_128;
      len  = len_128;
    }
    else
    {
      data = (void *)data_192;
      len  = len_192;
    }
    wxMemoryInputStream istream(data,len);
    img.LoadFile(istream);
  }
  if(!img.IsOk()) {
    img = wxImage(invalidImage_xpm);
  }

  img.Rescale(targetSize, targetSize, wxIMAGE_QUALITY_HIGH);
  return img;
}

ToolBar::~ToolBar()
{
  m_plotSlider = NULL;
  m_toolBar->Destroy();
}

void ToolBar::UpdateSlider(SlideShow *cell)
{
  if(cell == NULL)
    return;
  int slideShowDisplayedIndex = cell->GetDisplayedIndex();
  int slideShowMaxIndex = cell->Length();

  if ((m_slideShowDisplayedIndex != slideShowDisplayedIndex) || (m_slideShowMaxIndex != slideShowMaxIndex))
  {
    m_slideShowMaxIndex = slideShowMaxIndex;
    m_slideShowDisplayedIndex = slideShowDisplayedIndex;
    if (m_plotSlider != NULL)
    {
      m_plotSlider->SetRange(0, cell->Length() - 1);
      m_plotSlider->SetValue(cell->GetDisplayedIndex());
      m_plotSlider->SetToolTip(wxString::Format(_("Frame %i of %i"), cell->GetDisplayedIndex() + 1, cell->Length()));
    }
  }
}

ToolBar::ToolBar(wxToolBar *tbar)
{
  m_defaultCellStyle = GC_TYPE_CODE;
  m_canCopy_old = true;
  m_canCut_old = true;
  m_canSave_old = true;
  m_canPrint_old = true;
  m_canEvalTillHere_old = true;

  m_toolBar = tbar;
  m_needsInformation = false;
  m_AnimationStartStopState = Inactive;

  m_toolBar->SetToolBitmapSize(wxSize(24, 24));

#if defined __WXMSW__
  m_toolBar->AddTool(tb_new, _("New"),
                     GetImage(wxT("gtk-new"),
                              gtk_new_128_png,gtk_new_128_png_len,
                              gtk_new_192_png,gtk_new_192_png_len
                       ),
                     _("New document"));
#endif
  m_toolBar->AddTool(tb_open, _("Open"),
                     GetImage(wxT("gtk-open"),
                              gtk_open_128_png,gtk_open_128_png_len,
                              gtk_open_192_png,gtk_open_192_png_len
                       ),
                     _("Open document"));
  m_toolBar->AddTool(tb_save, _("Save"),
                     GetImage(wxT("gtk-save"),
                              gtk_save_128_png,gtk_save_128_png_len,
                              gtk_save_192_png,gtk_save_192_png_len
                       ),
                     _("Save document"));
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  m_toolBar->AddTool(tb_print, _("Print"),
                     GetImage(wxT("gtk-print"),
                              gtk_print_128_png,gtk_print_128_png_len,
                              gtk_print_192_png,gtk_print_192_png_len
                       ),
                     _("Print document"));
  m_toolBar->AddTool(tb_pref, _("Options"),
                     GetImage(wxT("gtk-preferences"),
                              gtk_preferences_128_png,gtk_preferences_128_png_len,
                              gtk_preferences_192_png,gtk_preferences_192_png_len
                       ),
                     _("Configure wxMaxima"));
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  m_toolBar->AddTool(tb_cut, _("Cut"),
                     GetImage(wxT("gtk-cut"),
                              gtk_cut_128_png,gtk_cut_128_png_len,
                              gtk_cut_192_png,gtk_cut_192_png_len
                       ),
                     _("Cut selection"));
  m_toolBar->AddTool(tb_copy, _("Copy"),
                     GetImage(wxT("gtk-copy"),
                              gtk_copy_128_png,gtk_copy_128_png_len,
                              gtk_copy_192_png,gtk_copy_192_png_len
                       ),
                     _("Copy selection"));
  m_toolBar->AddTool(tb_paste, _("Paste"),
                     GetImage(wxT("gtk-paste"),
                              gtk_paste_128_png,gtk_paste_128_png_len,
                              gtk_paste_192_png,gtk_paste_192_png_len
                       ),
                     _("Paste from clipboard"));
  m_toolBar->AddTool(tb_select_all, _("Select all"),
                     GetImage(wxT("gtk-select-all"),
                              gtk_select_all_128_png,gtk_select_all_128_png_len,
                              gtk_select_all_192_png,gtk_select_all_192_png_len
                       ),
                     _("Select all"));
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  m_toolBar->AddTool(tb_find, _("Find"),
                     GetImage(wxT("gtk-find"),
                              gtk_find_128_png,gtk_find_128_png_len,
                              gtk_find_192_png,gtk_find_192_png_len
                       ),
                     _("Find and replace"));
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  m_toolBar->AddTool(menu_restart_id, _("Restart maxima"),
                     GetImage(wxT("view-refresh"),
                              view_refresh_128_png,view_refresh_128_png_len,
                              view_refresh_192_png,view_refresh_192_png_len
                       ),
                     _("Completely stop maxima and restart it"));
  m_toolBar->AddTool(tb_interrupt, _("Interrupt"),
                     GetImage(wxT("gtk-stop"),
                              gtk_stop_128_png,gtk_stop_128_png_len,
                              gtk_stop_192_png,gtk_stop_192_png_len
                       ),
                     _("Interrupt current computation. To completely restart maxima press the button left to this one."));
  m_followIcon = GetImage(wxT("weather-clear"),
                              weather_clear_128_png,weather_clear_128_png_len,
                              weather_clear_192_png,weather_clear_192_png_len
    );
  m_needsInformationIcon = GetImage(wxT("software-update-urgent"),
                              software_update_urgent_128_png,software_update_urgent_128_png_len,
                              software_update_urgent_192_png,software_update_urgent_192_png_len
    );
  m_toolBar->AddTool(tb_follow, _("Follow"), m_followIcon,
                     _("Return to the cell that is currently being evaluated"));
  m_toolBar->EnableTool(tb_follow, false);

  m_toolBar->AddTool(tb_evaltillhere, _("Evaluate to point"),
                     GetImage(wxT("go-bottom"),
                              go_bottom_128_png,go_bottom_128_png_len,
                              go_bottom_192_png,go_bottom_192_png_len
                       ),
                     _("Evaluate the file from its beginning to the cell above the cursor"));

#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  wxArrayString textStyle;
  textStyle.Add(_("Maths"));
  textStyle.Add(_("Text"));
  textStyle.Add(_("Title"));
  textStyle.Add(_("Section"));
  textStyle.Add(_("Subsection"));
  textStyle.Add(_("Subsubsection"));
  m_textStyle = new wxChoice(m_toolBar, tb_changeStyle, wxDefaultPosition, wxDefaultSize, textStyle);
  m_textStyle->SetToolTip(_("For faster creation of cells the following shortcuts exist:\n\n"
                            "   Ctrl+0: Math cell\n"
                            "   Ctrl+1: Text cell\n"
                            "   Ctrl+2: Title cell\n"
                            "   Ctrl+3: Section cell\n"
                            "   Ctrl+4: Subsection cell\n"
                            "   Ctrl+5: Sub-Subsection cell"));
  m_toolBar->AddControl(m_textStyle);
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  
  // Seems like on MSW changing the image of this button has strange side-effects
  // so we combine both images into one for this OS.
#if defined __WXMSW__
  m_PlayButton = GetImage(wxT("media-playback-startstop"),
                              media_playback_startstop_128_png,media_playback_startstop_128_png_len,
                              media_playback_startstop_192_png,media_playback_startstop_192_png_len
    );
#else
  m_PlayButton = GetImage(wxT("media-playback-start"),
                              media_playback_start_128_png,media_playback_start_128_png_len,
                              media_playback_start_192_png,media_playback_start_192_png_len
    );
#endif
  m_StopButton = GetImage(wxT("media-playback-stop"),
                              media_playback_stop_128_png,media_playback_stop_128_png_len,
                              media_playback_stop_192_png,media_playback_stop_192_png_len
    );

  // It felt like a good idea to combine the play and the stop button.
  // On windows changing a button seems to somehow stop the animation, though, so
  // this OS requires the buttons to be separate.
  m_toolBar->AddTool(tb_animation_startStop, _("Start or Stop animation"),
                     m_PlayButton,
                     _("Start or stop the currently selected animation that has been created with the with_slider class of commands"));
  m_toolBar->EnableTool(tb_animation_startStop, false);
  int sliderWidth = wxGetDisplayPPI().x * 200 / 72;
  int width, height;
  wxDisplaySize(&width, &height);
  if (width < 800)
    sliderWidth = MIN(sliderWidth, 100);
  m_plotSlider = new wxSlider(m_toolBar, plot_slider_id, 0, 0, 10,
                              wxDefaultPosition, wxSize(sliderWidth, -1),
                              wxSL_HORIZONTAL | !wxSL_AUTOTICKS);
  m_plotSlider->SetToolTip(
          _("After clicking on animations created with with_slider_draw() or similar this slider allows to change the current frame."));
  m_plotSlider->Enable(false);
  m_slideShowMaxIndex = -1;
  m_slideShowDisplayedIndex = -1;
  m_toolBar->AddControl(m_plotSlider);
#ifndef __WXMAC__
  m_toolBar->AddSeparator();
#endif
  m_toolBar->AddTool(tb_hideCode, _("Hide Code"),
                     GetImage(wxT("weather-few-clouds"),
                              weather_few_clouds_128_png,weather_few_clouds_128_png_len,
                              weather_few_clouds_192_png,weather_few_clouds_192_png_len
                       ),
                     _("Toggle the visibility of code cells"));
  m_toolBar->AddStretchableSpace();
  m_toolBar->AddTool(tb_help, _("Help"),
                     GetImage(wxT("gtk-help"),
                              gtk_help_128_png,gtk_help_128_png_len,
                              gtk_help_192_png,gtk_help_192_png_len
                       ),
                     _("Show Maxima help"));
  m_toolBar->Realize();
}

void ToolBar::SetDefaultCellStyle()
{
  switch(m_textStyle->GetSelection())
  {
  case 0:
    m_defaultCellStyle = GC_TYPE_CODE;
    break;
  case 1:
    m_defaultCellStyle = GC_TYPE_TEXT;
    break;
  case 2:
    m_defaultCellStyle = GC_TYPE_TITLE;
    break;
  case 3:
    m_defaultCellStyle = GC_TYPE_SECTION;
    break;    
  case 4:
    m_defaultCellStyle = GC_TYPE_SUBSECTION;
    break;
  case 5:
    m_defaultCellStyle = GC_TYPE_SUBSUBSECTION;
    break;
  default:
  {}
  }
}

int ToolBar::GetCellStyle()
{
  switch(m_textStyle->GetSelection())
  {
  case 1:
    return GC_TYPE_TEXT;
    break;
  case 2:
    return GC_TYPE_TITLE;
    break;
  case 3:
    return GC_TYPE_SECTION;
    break;    
  case 4:
    return GC_TYPE_SUBSECTION;
    break;
  case 5:
    return GC_TYPE_SUBSUBSECTION;
    break;
  case 6:
    return GC_TYPE_IMAGE;
    break;
  case 7:
    return GC_TYPE_PAGEBREAK;
    break;
  default:
    return GC_TYPE_CODE;
  }
}

void ToolBar::SetCellStyle(int style)
{

  switch(style)
  {
  case GC_TYPE_CODE:
  case GC_TYPE_TEXT:
  case GC_TYPE_TITLE:
  case GC_TYPE_SECTION:
  case GC_TYPE_SUBSECTION:
  case GC_TYPE_SUBSUBSECTION:
    break;
  default:
    style = m_defaultCellStyle;
  }

  switch(style)
  {
  case GC_TYPE_CODE:
    m_textStyle->SetSelection(0);
    break;
  case GC_TYPE_TEXT:
    m_textStyle->SetSelection(1);
    break;
  case GC_TYPE_TITLE:
    m_textStyle->SetSelection(2);
    break;
  case GC_TYPE_SECTION:
    m_textStyle->SetSelection(3);
    break;
  case GC_TYPE_SUBSECTION:
    m_textStyle->SetSelection(4);
    break;
  case GC_TYPE_SUBSUBSECTION:
    m_textStyle->SetSelection(5);
    break;
  }
}


void ToolBar::AnimationButtonState(AnimationStartStopState state)
{
  if (m_AnimationStartStopState != state)
  {
    switch (state)
    {
      case Running:
        m_plotSlider->Enable(true);
        if (m_AnimationStartStopState != Running)
        {
#ifndef __WXMSW__
          m_toolBar->SetToolNormalBitmap(tb_animation_startStop, m_StopButton);
#endif
        }
        m_toolBar->EnableTool(tb_animation_startStop, true);
        break;
      case Stopped:
        if (m_AnimationStartStopState == Running)
        {
#ifndef __WXMSW__
          m_toolBar->SetToolNormalBitmap(tb_animation_startStop, m_PlayButton);
#endif
        }
        m_toolBar->EnableTool(tb_animation_startStop, true);
        m_plotSlider->Enable(true);
        break;
      case Inactive:
        m_toolBar->EnableTool(tb_animation_startStop, false);
        m_plotSlider->Enable(false);
        m_plotSlider->SetToolTip(
                _("After clicking on animations created with with_slider_draw() or similar this slider allows to change the current frame."));
        m_slideShowMaxIndex = -1;
        m_slideShowDisplayedIndex = -1;

        if (m_AnimationStartStopState == Running)
        {
#ifndef __WXMSW__
          m_toolBar->SetToolNormalBitmap(tb_animation_startStop, m_PlayButton);
#endif
        }
        break;
    }
    m_AnimationStartStopState = state;
  }
}
