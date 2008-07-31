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
#include "fontpage.h"

FontPage::FontPage( QWidget *parent ) : QWidget( parent )
{
    setupUi( parent );
//     connect( fontchooser, SIGNAL( fontSelected( const QFont& ) ), this, SIGNAL( fontSelected( const QFont& ) ) );
}

FontPage::~FontPage()
{
}

// QFont FontPage::font() const
// {
//     return fontchooser->font();
// }
// 
// void FontPage::setFont( const QFont &font, const bool onlyFixed )
// {
//     fontchooser->setFont( font, onlyFixed );
// }

#include "fontpage.moc"
