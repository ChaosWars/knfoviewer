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

#include <kurl.h>
#include <kparts/factory.h>
#include <kparts/part.h>

class KNFOViewerHTML;
class QHBoxLayout;

/**
 * @brief Main Part
 * This is a "Part".  It that does all the real work in a KPart application.
 * See the KDE KPart documentation.
 * @author Lawrence Lee <valher@facticius.net>
 * @version 0.3.4
 */
class KNFOViewerPart : public KParts::ReadOnlyPart
{
    Q_OBJECT

    public:
        KNFOViewerPart( QWidget *parentWidget, const char *widgetName,
                        QObject *parent, const char *name );
        virtual ~KNFOViewerPart();

    public slots:
        void loadSettings();

    protected:
        virtual bool openFile();
        virtual bool openURL( const KURL &url );

    protected slots:
        void fileOpen();
        void optionsConfigure();

    private:
        QFont font;
        QString text;
        QColor backgroundColor;
        QColor textColor;
        QColor linkColor;
        QWidget *m_widget;
        KNFOViewerHTML *htmlpart;
        void setupMainWidget();
        const QString htmlCode( const QString &text );
        void display();

    signals:
        void addRecentFile( const KURL& url );
};

class KInstance;
class KAboutData;

class KNFOViewerPartFactory : public KParts::Factory
{
    Q_OBJECT
    public:
        KNFOViewerPartFactory();
        virtual ~KNFOViewerPartFactory();
        virtual KParts::Part* createPartObject( QWidget *parentWidget, const char *widgetName,
                                                QObject *parent, const char *name,
                                                const char *classname, const QStringList &args );
        static KInstance* instance();

    private:
        static KInstance* s_instance;
        static KAboutData* s_about;
};

#endif // _KNFOVIEWER_PART_H_
