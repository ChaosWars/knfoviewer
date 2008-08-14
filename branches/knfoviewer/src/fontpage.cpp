/***************************************************************************
 *   Copyright (C) 2007 by Lawrence Lee                                    *
 *   valheru@facticius.net                                                 *
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

#include <kfontdialog.h>
#include <qlayout.h>
#include "fontpage.h"

FontPage::FontPage( QWidget *parent, const char *name, WFlags fl )
 : /*FontPageBase( parent, name, fl )*/QWidget( parent, name, fl )
{
    QHBoxLayout *layout = new QHBoxLayout( this );
    KFontChooser *fc = new KFontChooser( this, "fc", true );
    layout->addWidget( fc );
    connect( fc, SIGNAL( fontSelected( const QFont& ) ), this, SLOT( fontSelected( const QFont& ) ) );
}

FontPage::~FontPage()
{
}

void FontPage::fontSelected( const QFont& font )
{
    qDebug( "%s", font.family() );
//     kcfg_fontChooser->setFont( font, true );
}

#include "fontpage.moc"
