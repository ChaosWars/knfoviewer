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

#ifndef _NFOPREVIEW_H_
#define _NFOPREVIEW_H_

#include <kio/thumbcreator.h>

class KHTMLPart;

class KNfoViewerPreview : public QObject, public ThumbCreator
{
    Q_OBJECT

    public:
        KNfoViewerPreview();
        ~KNfoViewerPreview();
        virtual bool create( const QString &path, int width, int height, QImage &img );
        virtual Flags flags() const;

    protected:
        virtual void timerEvent(QTimerEvent *);

    private slots:
        void slotCompleted();

    private:
        KHTMLPart *m_html;
};

#endif
