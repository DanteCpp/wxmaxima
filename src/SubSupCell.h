// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
//  Copyright (C) 2007-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2014-2015 Gunter Königsmann <wxMaxima@physikbuch.de>
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

#ifndef SUBSUPCELL_H
#define SUBSUPCELL_H

#include "MathCell.h"

class SubSupCell : public MathCell
{
public:
  SubSupCell();
  ~SubSupCell();
  MathCell* Copy();
  void Destroy();
  void SetBase(MathCell *base);
  void SetIndex(MathCell *index);
  void SetExponent(MathCell *expt);
  void SetPreSub(MathCell *presub);
  void SetPreSup(MathCell *presup);
  void RecalculateSize(CellParser& parser, int fontsize);
  void RecalculateWidths(CellParser& parser, int fontsize);
  void Draw(CellParser& parser, wxPoint point, int fontsize);
  wxString ToString();
  wxString ToTeX();
  wxString ToXML();
  void SelectInner(wxRect& rect, MathCell** first, MathCell** last);
  void SetParent(MathCell *parent);
protected:
  //! The name that is to be equipped with super-and subscripts
  MathCell *m_baseCell;
  //! The superscript
  MathCell *m_exptCell;
  //! The subscript
  MathCell *m_indexCell;
  //! A superscript displayed in front of the cell
  MathCell *m_presupCell;
  //! A subscript displayed in front of the cell
  MathCell *m_presubCell;
};

#endif // SUBSUPCELL_H
