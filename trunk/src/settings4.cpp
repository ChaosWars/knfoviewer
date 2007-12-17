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
#include <KDE/KLocale>
#include "settings4.h"
#include "colorpage4.h"
#include "fontpage4.h"
#include "knfoviewersettings.h"

Settings4::Settings4( QWidget *parent, const char *name, KNfoViewerSettings *config )
    : KConfigDialog( parent, name, config ), m_config( config ), fc( false ), cc( false )
{
    //Set up font page
    fontPage = new FontPage4();
    fontPage->setFont( config->font(), true );
    connect( fontPage, SIGNAL( fontSelected( const QFont& ) ), this, SLOT( fontChanged( const QFont& ) ) );
    addPage( fontPage, ki18n( "Configure Fonts" ), "fonts" );

    //Set up the color chooser page
    colorPage = new ColorPage4();
    connect( colorPage->backgroundColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( backgroundColorChanged( const QColor& ) ) );
    connect( colorPage->textColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( textColorChanged( const QColor& ) ) );
    connect( colorPage->linkColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( linkColorChanged( const QColor& ) ) );
    colorPage->setBackgroundColor( config->backgroundColor()  );
    colorPage->setTextColor( config->textColor()  );
    colorPage->setLinkColor( config->linkColor()  );
    addPage( colorPage, ki18n( "Configure Colors" ), "colorize" );
}

Settings4::~Settings4()
{
}

void Settings4::fontChanged( const QFont &chosenFont )
{
    if( chosenFont != font ){
        fc = true;
        enableButton( Apply, true );
    }
}

void Settings4::backgroundColorChanged( const QColor &color )
{
    if( backgroundColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings4::textColorChanged( const QColor &color )
{
    if( textColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings4::linkColorChanged( const QColor &color )
{
    if( linkColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings4::updateSettings()
{
    bool settingsChanged = false;

    if( fc ){
        font = fontChooser->font();
        m_config->setFont( font );
        fc = false;
        settingsChanged = true;
    }

    if( cc ){
        backgroundColor = colorPage->backgroundColor();
        m_config->setBackgroundColor( backgroundColor );
        textColor = colorPage->textColor();
        m_config->setTextColor( textColor );
        linkColor = colorPage->linkColor();
        m_config->setLinkColor( linkColor );
        cc = false;
        settingsChanged = true;
    }

    if( settingsChanged ){
        m_config->writeConfig();
        settingsChangedSlot();
    }
}

#include "settings4.moc"
