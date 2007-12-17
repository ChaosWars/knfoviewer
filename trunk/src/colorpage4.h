/***************************************************************************
 *   Copyright (C) 2007 by Lawrence Lee   *
 *   valheru@facticius.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _COLORPAGE4_H_
#define _COLORPAGE4_H_

#include <KDE/KColorCombo>
#include <QColor>
#include "ui_color4.h"

class ColorPage4 : public QWidget, public Ui::ColorPage4
{
    public:
        ColorPage4(){ setupUi( this ); }
        ~ColorPage4(){}
        KColorCombo *backgroundColorCombo(){ return m_BackgroundColorCombo ; }
        KColorCombo *textColorCombo(){ return m_TextColorCombo ; }
        KColorCombo *linkColorCombo(){ return m_LinkColorCombo ; }
        QColor backgroundColor() const{ return m_BackgroundColorCombo->color(); };
        QColor textColor() const{ return m_TextColorCombo->color(); };
        QColor linkColor() const{ return m_LinkColorCombo->color(); };
};

#endif