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
#include "settings.h"
#include "colorpage.h"
#include "fontpage.h"
#include "knfoviewersettings.h"

Settings::Settings( QWidget *parent, const char *name, KNfoViewerSettings *config )
    : KConfigDialog( parent, name, config ), m_config( config ), fc( false ), cc( false )
{
    //Set up font page
    fontPage = new FontPage();
    fontPage->setFont( config->font(), true );
    connect( fontPage, SIGNAL( fontSelected( const QFont& ) ), this, SLOT( fontChanged( const QFont& ) ) );
    addPage( fontPage, "configure_fonts", "fonts", "Configure Fonts" );

    //Set up the color chooser page
    colorPage = new ColorPage();
    connect( colorPage->backgroundColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( backgroundColorChanged( const QColor& ) ) );
    connect( colorPage->textColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( textColorChanged( const QColor& ) ) );
    connect( colorPage->linkColorCombo(), SIGNAL( activated( const QColor& ) ), this, SLOT( linkColorChanged( const QColor& ) ) );
    colorPage->setBackgroundColor( config->backgroundColor()  );
    colorPage->setTextColor( config->textColor()  );
    colorPage->setLinkColor( config->linkColor()  );
    addPage( colorPage, "configure_colors", "colorize", "Configure Colors" );
}

Settings::~Settings()
{
}

void Settings::fontChanged( const QFont &chosenFont )
{
    if( chosenFont != font ){
        fc = true;
        enableButton( Apply, true );
    }
}

void Settings::backgroundColorChanged( const QColor &color )
{
    if( backgroundColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings::textColorChanged( const QColor &color )
{
    if( textColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings::linkColorChanged( const QColor &color )
{
    if( linkColor != color ){
        cc = true;
        enableButton( Apply, true );
    }
}

void Settings::updateSettings()
{
    bool settingsChanged = false;

    if( fc ){
        font = fontPage->font();
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
    }
}

#include "settings.moc"