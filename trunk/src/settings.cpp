/***************************************************************************
 *   Copyright (C) 2007 by Lawrence Lee   *
 *   valheru@facticius.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
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
#include <klocale.h>
#include <qlayout.h>
#include "settings.h"
#include "knfoviewersettings.h"
#include "color.h"

Settings::Settings( QWidget *parent, const char *name, KNfoViewerSettings *config )
    : KConfigDialog( parent, name, config ), m_config( config )
{
    //Set up font page
    fontPage = new QWidget( 0, "FontPage" );
    fontLayout = new QGridLayout( fontPage );
    fontChooser = new KFontChooser( fontPage, "FontChooser", true );
    font.fromString( m_config->font() );
    fontChooser->setFont( font, true );
    fontLayout->addWidget( fontChooser, 0, 0 );
    connect( fontChooser, SIGNAL( fontSelected( const QFont& ) ), this, SLOT( fontChanged( const QFont& ) ) );
    addPage( fontPage, i18n( "Configure Fonts" ), "fonts" );
}

Settings::~Settings()
{
}

void Settings::fontChanged( const QFont &chosenFont )
{
    if( chosenFont != font ){
        newFont = chosenFont;
        enableButton( Apply, true );
    }
}

void Settings::updateSettings()
{
    bool settingsChanged = false;

    if( newFont != font ){
        font = newFont;
        m_config->setFont( font.toString() );
        settingsChanged = true;
    }

    if( settingsChanged ){
        m_config->writeConfig();
        settingsChangedSlot();
    }
}

#include "settings.moc"
