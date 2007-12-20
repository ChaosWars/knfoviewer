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

#ifndef _KNFOVIEWER4_H_
#define _KNFOVIEWER4_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <KDE/KParts/MainWindow>
#include <KDE/KUrl>

class KToggleAction;
class KRecentFilesAction;

/**
 * This is the application "Shell".  It has a menubar, toolbar, and
 * statusbar but relies on the "Part" to do all the real work.
 *
 * @short Application Shell
 * @author Lawrence Lee <valher@facticius.net>
 * @version 0.3.1
 */
class KNfoViewer4 : public KParts::MainWindow
{
    Q_OBJECT

    public:
    /**
     * Default Constructor
     */
        KNfoViewer4();

    /**
         * Default Destructor
     */
        virtual ~KNfoViewer4();

    /**
         * Use this method to load whatever file/URL you have
     */
        void load(const KUrl& url);

    protected:

    /**
     * This method is called when it is time for the app to save its
     * properties for session management purposes.
     */
        void saveProperties( KConfigGroup &config );

    /**
         * This method is called when this app is restored.  The KConfig
         * object points to the session management config file that was saved
         * with @ref saveProperties
     */
        void readProperties( const KConfigGroup &config );

    private slots:
        void optionsConfigureKeys();
        void optionsConfigureToolbars();
        void applyNewToolbarConfig();
        void addRecentFile( const KUrl &url );
        void openRecent( const KUrl &url );

    private:
        KConfig *config;
        KRecentFilesAction *recentFiles;
        KParts::ReadOnlyPart *m_part;
        void setupAccel();
        void setupActions();
};

#endif
