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

#ifndef _KNFOVIEWER_PART_H_
#define _KNFOVIEWER_PART_H_

#include <KDE/KParts/ReadOnlyPart>
#include <KDE/KUrl>

class KAction;
class KAboutData;
class KNfoViewerHTML;
class KRecentFilesAction;
class KConfigDialog;
class KNfoViewerSettings;
class QGridLayout;
class MainWidget;


/**
 * This is a "Part".  It that does all the real work in a KPart
 * application.
 *
 * @short Main Part
 * @author Lawrence Lee <valher@facticius.net>
 * @version 0.3.2
 */
class KNfoViewerPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

    public:
    /**
     * Default constructor
     */
        KNfoViewerPart( QWidget* parentWidget,
                         QObject* parent,
                         const QStringList& args = QStringList() );

    /**
         * Destructor
     */
        virtual ~KNfoViewerPart();
        static KAboutData* createAboutData();

    public Q_SLOTS:
        void loadSettings();

    protected:
    /**
     * This must be implemented by each part
     */
        virtual bool openFile();
        virtual bool openUrl( const KUrl & url );

    protected Q_SLOTS:
        void fileOpen();
        void configureSettings();

    private:
        QFont font;
        QString text;
        QColor backgroundColor;
        QColor textColor;
        QColor linkColor;
        KAction *configureAction;
        KConfigDialog *settings;
        KNfoViewerSettings *config;
        MainWidget *m_widget;
        KNfoViewerHTML *htmlpart;
        QGridLayout *layout;
        void setupMainWidget();
        const QString htmlCode( const QString &text );
        void saveProperties( KNfoViewerSettings *config );
        void readProperties( KNfoViewerSettings *config );
        void display();

    Q_SIGNALS:
        void addRecentFile( const KUrl& url );
};

// #include <KDE/KParts/Factory>
//
// class KNfoViewerPartFactory : public KParts::Factory
// {
//     Q_OBJECT
//     public:
//         KNfoViewerPartFactory();
//         virtual ~KNfoViewerPartFactory();
//         virtual KParts::Part* createPartObject( QWidget *parentWidget, const char *widgetName,
//                 QObject *parent, const char *name,
//                 const char *classname, const QStringList &args );
//         static KInstance* instance();
//
//     private:
//         static KInstance* s_instance;
//         static KAboutData* s_about;
// };

#endif // _KNFOVIEWER_PART_H_
