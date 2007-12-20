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
#include <KDE/KMessageBox>
#include <KDE/KPluginLoader>
#include <KDE/KPluginFactory>
#include <KDE/KFileDialog>
#include <KDE/KConfig>
#include <KDE/KEditToolBar>
#include <KDE/KRecentFilesAction>
#include <KDE/KStandardAction>
#include <KDE/KActionCollection>
#include <KDE/KStatusBar>
#include <KDE/KLocale>
#include <kdebug.h>
#include "knfoviewer.h"

KNfoViewer::KNfoViewer()
{
    // set the shell's ui resource file
    setXMLFile("knfoviewer.rc");

    // then, setup our actions
    setupActions();

    KPluginLoader loader( "knfoviewerpart" );
    KPluginFactory* factory = loader.factory();

    if( !factory ){
        kWarning() << "Error loading plugin:" << loader.errorString();
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
        connect( m_part->widget(), SIGNAL( urlMouseOver( const QString& ) ), statusBar(), SLOT( message( const QString& ) ) );

        config = KGlobal::config();
        //readProperties( &config );

        // apply the saved mainwindow settings, if any, and ask the mainwindow
        // to automatically save settings if changed: window size, toolbar
        // position, icon size, etc.
        setAutoSaveSettings();

    }

}

KNfoViewer::~KNfoViewer()
{
     //saveProperties( &config );
}

void KNfoViewer::load( const KUrl& url )
{
     m_part->openUrl( url );
}

void KNfoViewer::addRecentFile( const KUrl &url )
{
    recentFiles->addUrl( url );
}

void KNfoViewer::openRecent( const KUrl &url )
{
     m_part->openUrl( url );
}

void KNfoViewer::setupActions()
{
    setStandardToolBarMenuEnabled( true );
    createStandardStatusBarAction();
    KStandardAction::quit( this, SLOT( close() ), actionCollection() );
    KStandardAction::keyBindings( this, SLOT( optionsConfigureKeys()), actionCollection() );
    KStandardAction::configureToolbars( this, SLOT( optionsConfigureToolbars()), actionCollection() );
    recentFiles = KStandardAction::openRecent( this, SLOT( openRecent( const KUrl& ) ), actionCollection() );
}

void KNfoViewer::saveProperties( KConfigGroup& )
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
//      config->setGroup( "RecentFiles" );
//      recentFiles->saveEntries( &config );
}

void KNfoViewer::readProperties( const KConfigGroup& )
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'
//      config->setGroup( "RecentFiles" );
//      recentFiles->loadEntries( &config );
}

void KNfoViewer::optionsConfigureKeys()
{
//     KKeyDialog::configure( actionCollection() );
}

void KNfoViewer::optionsConfigureToolbars()
{
    // use the standard toolbar editor
    KEditToolBar dlg(factory());
    connect( &dlg, SIGNAL( newToolbarConfig() ), this, SLOT( applyNewToolbarConfig() ) );
    dlg.exec();
}

void KNfoViewer::applyNewToolbarConfig()
{
}

#include "knfoviewer.moc"
