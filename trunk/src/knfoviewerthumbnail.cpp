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
#include <kstandarddirs.h>
#include <kmimetype.h>
#include <kpixmapsplitter.h>
#include <qfile.h>
#include <qimage.h>
#include <qtextstream.h>
#include "knfoviewerthumbnail.h"
#include "cp437codec.h"

extern "C"
{
    ThumbCreator *new_creator()
    {
        return new KNfoViewerThumbnail();
    }
}

/*
 * This code is taken nearly verbatim from textcreator.cpp
 * in kdebase/kioslave/thumbnail/ with minor changes to
 * ensure the correct rendering of CP437 encoded characters.
 */
KNfoViewerThumbnail::KNfoViewerThumbnail() : m_splitter( 0 )
{
}

KNfoViewerThumbnail::~KNfoViewerThumbnail()
{
    delete m_splitter;
}

bool KNfoViewerThumbnail::create( const QString &path, int width, int height, QImage &img )
{
    // Filter out unwanted mimetypes
    KMimeType::Ptr mimeType = KMimeType::findByPath( path );

    if( mimeType->is( "text/x-nfo" ) )
        return false;

    QFile file( path );

    if( !file.open( IO_ReadOnly ) )
         return false;

    QTextStream stream( &file );
    CP437Codec codec;
    stream.setCodec( &codec );
    QString currentLine;
    QString text;
    int maxLineLength = 0;
    int numLines = 0;

    while( !stream.atEnd() ){
        currentLine = stream.readLine();
        int currentLineLength = currentLine.length();

        if( currentLineLength > maxLineLength )
            maxLineLength = currentLineLength;

        text += currentLine + "\n";
        numLines++;
    }

    file.close();

    if ( !m_splitter ){
        m_splitter = new KPixmapSplitter;
        QString pixmap = locate( "data", "konqueror/pics/thumbnailfont_7x4.png" );

        if ( !pixmap.isEmpty() ){
            // FIXME: make font/glyphsize configurable...
            m_splitter->setPixmap( QPixmap( pixmap ) );
            m_splitter->setItemSize( QSize( 4, 7 ) );
        }

    }

    QSize chSize = m_splitter->itemSize(); // the size of one char
    int xOffset = chSize.width();
    int yOffset = chSize.height();

    width = xOffset * maxLineLength + 2*( 1 + maxLineLength/16 );
    height = yOffset + numLines + 2*( 1 + numLines/16 );
    width > 400 ? width = 400 : width;
    height > 600 ? height = 600 : height;
    QSize pixmapSize( width, height );
    m_pixmap.resize( pixmapSize );
    m_pixmap.fill( QColor( 245, 245, 245 ) ); // light-grey background

    // one pixel for the rectangle, the rest. whitespace
    int xborder = 1 + pixmapSize.width()/16;  // minimum x-border
    int yborder = 1 + pixmapSize.height()/16; // minimum y-border
    int rest = m_pixmap.width() - ( maxLineLength * chSize.width() );
    xborder = QMAX( xborder, rest/2 ); // center horizontally
    rest = m_pixmap.height() - ( numLines * chSize.height() );
    yborder = QMAX( yborder, rest/2 ); // center vertically
    // end centering

    int x = xborder, y = yborder; // where to paint the characters
    const QPixmap *fontPixmap = &(m_splitter->pixmap());
    QRect rect;

    for( int i = 0; i < numLines; i++ ){
        QChar ch = text.at( i );

        if ( ch == '\n' ){
            x = xborder;
            y += yOffset;
            continue;
        }else{
            rect = m_splitter->coordinates( ch );

            if ( !rect.isEmpty() ){
                bitBlt( &m_pixmap, QPoint( x,y ), fontPixmap, rect, Qt::CopyROP );
            }

            x += xOffset; // next character
        }

    }

    img = m_pixmap.convertToImage();
    return true;
}

ThumbCreator::Flags KNfoViewerThumbnail::flags() const
{
    return DrawFrame;
}

#include "knfoviewerthumbnail.moc"
