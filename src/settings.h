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
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <KDE/KConfigDialog>

class KNfoViewerSettings;
class FontPage;
class ColorPage;

class Settings : public KConfigDialog
{
    Q_OBJECT

    public:
        Settings( QWidget *parent, const char *name, KNfoViewerSettings *config );
        ~Settings();

    protected Q_SLOTS:
        virtual void updateSettings();

    private:
        QFont font;
        QColor backgroundColor;
        QColor textColor;
        QColor linkColor;
        KNfoViewerSettings *m_config;
        bool fc; //Fonts changed
        bool cc; //Colors changed
        FontPage *fontPage;
        ColorPage *colorPage;

    private Q_SLOTS:
        void fontChanged( const QFont& newFont );
        void backgroundColorChanged( const QColor &color );
        void textColorChanged( const QColor &color );
        void linkColorChanged( const QColor &color );

    Q_SIGNALS:
        void settingsChanged();
};

#endif
