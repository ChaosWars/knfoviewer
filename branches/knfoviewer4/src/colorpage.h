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
#ifndef _COLORPAGE_H_
#define _COLORPAGE_H_

#include <KDE/KColorCombo>
#include <QColor>
#include "ui_colorpage.h"

class ColorPage : public QWidget, public Ui::ColorPage
{
    public:
        ColorPage(){ setupUi( this ); }
        ~ColorPage(){}
        KColorCombo *backgroundColorCombo(){ return m_BackgroundColorCombo ; }
        KColorCombo *textColorCombo(){ return m_TextColorCombo ; }
        KColorCombo *linkColorCombo(){ return m_LinkColorCombo ; }
        QColor backgroundColor() const{ return m_BackgroundColorCombo->color(); };
        QColor textColor() const{ return m_TextColorCombo->color(); };
        QColor linkColor() const{ return m_LinkColorCombo->color(); };
        void setBackgroundColor( const QColor &color ){ m_BackgroundColorCombo->setColor( color ); }
        void setTextColor( const QColor &color ){ m_TextColorCombo->setColor( color ); }
        void setLinkColor( const QColor &color ){ m_LinkColorCombo->setColor( color ); }
};

#endif
