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
#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include "knfoviewer.h"

static const char description[] =
    I18N_NOOP( "A NFO viewer for KDE" );

static const char version[] = "0.3.4";

static KCmdLineOptions options[] =
{
    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
                           KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about( "knfoviewer", I18N_NOOP("KNFOViewer"), version, description, KAboutData::License_GPL,
                      "(C) 2007-2008 Lawrence Lee", 0, "http://knfoviewer.googlecode.com", "http://code.google.com/p/knfoviewer/issues/list" );
    about.addAuthor( "Lawrence Lee", 0, "valheru.ashen.shugar@gmail.com" );
    KCmdLineArgs::init( argc, argv, &about );
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;

    // see if we are starting with session management
    if ( app.isRestored() ){
        RESTORE( KNFOViewer );
    }else{
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        if ( args->count() == 0 ){
            KNFOViewer *widget = new KNFOViewer();
            widget->show();
        }else{

            for (int i = 0; i < args->count(); i++ )
            {
                KNFOViewer *widget = new KNFOViewer();
                widget->show();
                widget->load( args->url( i ) );
            }

        }
        args->clear();
    }

    return app.exec();
}
