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
#include <qfile.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtextstream.h>
#include <kstandarddirs.h>
#include <kpixmapsplitter.h>
#include <kmimetype.h>
#include <kdebug.h>
#include "knfoviewerpreview.h"
#include "cp437codec.h"

extern "C"
{
    ThumbCreator *new_creator()
    {
        return new KNfoViewerPreview();
    }
}

/*
 * This code is taken nearly verbatim from textcreator.cpp
 * in kdebase/kioslave/thumbnail/ with minor changes to
 * ensure the correct rendering of CP437 encoded characters.
 */
KNfoViewerPreview::KNfoViewerPreview() : m_splitter(0)
{
}

KNfoViewerPreview::~KNfoViewerPreview()
{
    delete m_splitter;
}

bool KNfoViewerPreview::create( const QString &path, int width, int height, QImage &img )
{
    // Filter out unwanted mimetypes
    KMimeType::Ptr mimeType = KMimeType::findByPath( path );
    if ( !mimeType->is( "text/x-nfo" ) )
        return false;

    if ( !m_splitter )
    {
        m_splitter = new KPixmapSplitter;
        QString pixmap = locate( "data", "konqueror/pics/thumbnailfont_7x4.png" );
        if ( !pixmap.isEmpty() )
        {
            // FIXME: make font/glyphsize configurable
            m_splitter->setPixmap( QPixmap( pixmap ));
            m_splitter->setItemSize( QSize( 4, 7 ));
        }
    }

    // determine some sizes...
    // example: width: 60, height: 64
    QSize pixmapSize( width, height );
    if (height * 3 > width * 4)
        pixmapSize.setHeight( width * 4 / 3 );
    else
        pixmapSize.setWidth( height * 3 / 4 );

    if ( pixmapSize != m_pixmap.size() )
        m_pixmap.resize( pixmapSize );

    // create text-preview
    QFile file( path );

    if ( file.open( IO_ReadOnly ) ){
        QTextStream stream( &file );
        CP437Codec codec;
        stream.setCodec( &codec );
        QString text;

        while( !stream.atEnd() ){
            QString s = stream.readLine();
            text += s + "\n";
        }

        file.close();

        m_pixmap.fill( QColor( 245, 245, 245 ) ); // light-grey background

        QRect rect;
        const QPixmap *fontPixmap = &(m_splitter->pixmap());

        for ( uint i = 0; i < text.length(); i++ ){
            QChar ch = text.at( i );
            rect = m_splitter->coordinates( ch );

            if ( !rect.isEmpty() ){
                bitBlt( &m_pixmap, QPoint(x,y), fontPixmap, rect, Qt::CopyROP );
            }

        }
            img = m_pixmap.convertToImage();
    }else{
       return false;
   }
    return true;
}

ThumbCreator::Flags KNfoViewerPreview::flags() const
{
    return (Flags)(DrawFrame | BlendIcon);
}
