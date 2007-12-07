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

#include <kkeydialog.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kedittoolbar.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <kstatusbar.h>
#include <klocale.h>
#include <kfontdialog.h>
#include "knfoviewer.h"

KNfoViewer::KNfoViewer()
    : KParts::MainWindow( 0L, "KNfoViewer" )
{
    // set the shell's ui resource file
    setXMLFile("knfoviewer_shell.rc");

    // then, setup our actions
    setupActions();

    // this routine will find and load our Part.  it finds the Part by
    // name which is a bad idea usually.. but it's alright in this
    // case since our Part is made for this Shell
    KLibFactory *factory = KLibLoader::self()->factory("libknfoviewerpart");

    if ( factory ){
        // now that the Part is loaded, we cast it to a Part to get
        // our hands on it
        m_part = static_cast<KParts::ReadOnlyPart *>(factory->create(this,
                "knfoviewer_part", "KParts::ReadOnlyPart" ));

        if( m_part ){
            // tell the KParts::MainWindow that this is indeed the main widget
            setCentralWidget(m_part->widget());

            // and integrate the part's GUI with the shell's
            createGUI(m_part);
        }

        connect( this, SIGNAL( setBrowserFont( const QFont& ) ), m_part, SLOT( setBrowserFont( const QFont& ) ) );
        connect( m_part, SIGNAL( addRecentFile( const KURL& ) ), this, SLOT( addRecentFile( const KURL&  ) ) );
    }
    else
    {
        // if we couldn't find our Part, we exit since the Shell by
        // itself can't do anything useful
        KMessageBox::error( this, i18n( "Could not find our part." ) );
        kapp->quit();
        // we return here, cause kapp->quit() only means "exit the
        // next time we enter the event loop...
        return;
    }

    config = KGlobal::config();
    readProperties( config );

    // apply the saved mainwindow settings, if any, and ask the mainwindow
    // to automatically save settings if changed: window size, toolbar
    // position, icon size, etc.
    setAutoSaveSettings();
}

KNfoViewer::~KNfoViewer()
{
}

void KNfoViewer::load( const KURL& url )
{
    m_part->openURL( url );
}

void KNfoViewer::addRecentFile( const KURL &url )
{
    recentFiles->addURL( url );
}

void KNfoViewer::openRecent( const KURL &url )
{
    m_part->openURL( url );
}

void KNfoViewer::setupActions()
{
    setStandardToolBarMenuEnabled( true );
    createStandardStatusBarAction();
    KStdAction::quit( kapp, SLOT( quit() ), actionCollection() );
    KStdAction::keyBindings( this, SLOT( optionsConfigureKeys()), actionCollection() );
    KStdAction::configureToolbars( this, SLOT( optionsConfigureToolbars()), actionCollection() );
    (void) new KAction( i18n( "&Configure Fonts" ), 0,
                        this, SLOT( optionsConfigureFonts() ),
                        actionCollection(), "options_configure_fonts" );
    recentFiles = KStdAction::openRecent( this, SLOT( openRecent( const KURL& ) ), actionCollection() );
}

void KNfoViewer::saveProperties( KConfig* config )
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored
    recentFiles->saveEntries( config );
}

void KNfoViewer::readProperties( KConfig* config )
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'
    recentFiles->loadEntries( config );
}

void KNfoViewer::optionsConfigureKeys()
{
    KKeyDialog::configure(actionCollection());
}

void KNfoViewer::optionsConfigureToolbars()
{
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
# else
    saveMainWindowSettings( KGlobal::config() );
# endif
#else
    saveMainWindowSettings( KGlobal::config() );
#endif

    // use the standard toolbar editor
    KEditToolbar dlg(factory());
    connect( &dlg, SIGNAL( newToolbarConfig() ), this, SLOT( applyNewToolbarConfig() ) );
    dlg.exec();
}

void KNfoViewer::optionsConfigureFonts()
{
    KFontDialog fd( 0L, 0, true );
    fd.setFont( m_part->widget()->font(), true );
    int result = fd.exec();

    if ( result == KFontDialog::Accepted ){
        emit setBrowserFont( fd.font() );
    }
}

void KNfoViewer::applyNewToolbarConfig()
{
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
# else
    applyMainWindowSettings(KGlobal::config());
# endif
#else
    applyMainWindowSettings(KGlobal::config());
#endif
}

#include "knfoviewer.moc"
