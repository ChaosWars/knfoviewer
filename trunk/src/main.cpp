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
#ifdef COMPILE_FOR_KDE4

#include <KDE/KLocalizedString>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>
#include <KDE/KApplication>
#include "knfoviewer.h"

static const char description[] ="A NFO viewer for KDE3";

static const char version[] = "0.3.1";

};

int main(int argc, char **argv)
{
    KCmdLineOptions options;
    options.add( "+files", ki18n( "Documents to open. You can list multiple files here." ) );
    KAboutData about( "KNfoViewer", QByteArray(), "KNfoViewer", version, ki18n( "A NFO viewer for KDE" ),
                        KAboutData::License_GPL, ki18n( "(C) 2007 Lawrence Lee" ), ki18n(""),
                        "http://knfoviewer.googlecode.com", "valheru@facticius.net" );
    about.addAuthor( "Lawrence Lee", ki18n( "Lead programmer" ), "valheru@facticius.net", "www.facticius.net" );

#else

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include "knfoviewer.h"

static const char description[] =
    I18N_NOOP( "A NFO viewer for KDE3" );

static const char version[] = "0.3.1";

static KCmdLineOptions options[] =
{
    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
                           KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about( "knfoviewer", I18N_NOOP("KNfoViewer"), version, description,
                      KAboutData::License_GPL, "(C) 2007 Lawrence Lee", 0, "http://knfoviewer.googlecode.com" );
    about.addAuthor( "Lawrence Lee", 0, "valheru@facticius.net" );

#endif



    KCmdLineArgs::init( argc, argv, &about );
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;

    // see if we are starting with session management
    if ( app.isRestored() ){
        RESTORE( KNfoViewer );
    }else{
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        if ( args->count() == 0 ){
            KNfoViewer *widget = new KNfoViewer();
            widget->show();
        }else{

            for (int i = 0; i < args->count(); i++ )
            {
                KNfoViewer *widget = new KNfoViewer();
                widget->show();
                widget->load( args->url( i ) );
            }

        }
        args->clear();
    }

    return app.exec();
}
