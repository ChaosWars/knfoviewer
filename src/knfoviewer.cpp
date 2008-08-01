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
#include <KDE/KApplication>
#include <KDE/KActionCollection>
#include <KDE/KConfig>
#include <KDE/KEditToolBar>
#include <KDE/KFileDialog>
#include <KDE/KLocale>
#include <KDE/KMessageBox>
#include <KDE/KPluginFactory>
#include <KDE/KPluginLoader>
#include <KDE/KRecentFilesAction>
#include <KDE/KStandardAction>
#include <KDE/KStatusBar>
#include "knfoviewer.h"

KNFOViewer::KNFOViewer()
    : KParts::MainWindow(), recentFiles( NULL ), m_part( NULL )
{
    setupActions();
    setupGUI( ToolBar | Keys | StatusBar );
    KPluginLoader loader( "knfoviewerpart" );
    KPluginFactory* factory = loader.factory();

    if( !factory ){
        KMessageBox::error( this, i18n( "Error loading plugin: %1", loader.errorString() ) );
    }else{

        m_part = factory->create< KParts::ReadOnlyPart >();

        if ( !m_part ) {
            // if we couldn't find our Part, we exit since the Shell by
            // itself can't do anything useful
            KMessageBox::error( this, i18n( "Could not find our part." ) );
            kapp->quit();
            // we return here, cause kapp->quit() only means "exit the
            // next time we enter the event loop...
            return;
        }

        // tell the KParts::MainWindow that this is indeed the main widget
        setCentralWidget( m_part->widget() );

        // and integrate the part's GUI with the shell's
        createGUI( m_part );

        connect( m_part, SIGNAL( addRecentFile( const KUrl& ) ), this, SLOT( addRecentFile( const KUrl&  ) ) );

        config = KGlobal::config();
        readProperties( KConfigGroup( config, "RecentFiles" ) );
        // apply the saved mainwindow settings, if any, and ask the mainwindow
        // to automatically save settings if changed: window size, toolbar
        // position, icon size, etc.
        setAutoSaveSettings();

    }

}

KNFOViewer::~KNFOViewer()
{
    KConfigGroup configGroup( config, "RecentFiles" );
    saveProperties( configGroup );
}

void KNFOViewer::load( const KUrl& url )
{
    if( m_part )
        m_part->openUrl( url );
}

void KNFOViewer::addRecentFile( const KUrl &url )
{
    recentFiles->addUrl( url );
}

void KNFOViewer::openRecentFile( const KUrl &url )
{
    if( m_part )
        m_part->openUrl( url );
}

void KNFOViewer::setupActions()
{
    setStandardToolBarMenuEnabled( true );
    createStandardStatusBarAction();
    recentFiles = KStandardAction::openRecent( this, SLOT( openRecentFile( const KUrl& ) ), actionCollection() );
    KStandardAction::quit( kapp, SLOT( closeAllWindows() ), actionCollection() );
}

void KNFOViewer::saveProperties( KConfigGroup &configGroup )
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
     recentFiles->saveEntries( configGroup );
}

void KNFOViewer::readProperties( const KConfigGroup &configGroup )
{
     recentFiles->loadEntries( configGroup );
}

#include "knfoviewer.moc"
