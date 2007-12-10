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
#include <kapplication.h>
#include <khtml_part.h>
#include <kstandarddirs.h>
#include <kmimetype.h>
#include <qfile.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qtextstream.h>
#include <time.h>
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
 * This code is taken nearly verbatim from htmlcreator.cpp
 * in kdebase/kioslave/thumbnail/ with minor changes to
 * ensure the correct rendering of CP437 encoded characters.
 */
KNfoViewerPreview::KNfoViewerPreview() : m_html( 0 )
{
}

KNfoViewerPreview::~KNfoViewerPreview()
{
    delete m_html;
}

bool KNfoViewerPreview::create( const QString &path, int width, int height, QImage &img )
{
    // Filter out unwanted mimetypes
    KMimeType::Ptr mimeType = KMimeType::findByPath( path );

    if( mimeType->is( "text/x-nfo" ) )
        return false;

    if (!m_html)
    {
        m_html = new KHTMLPart;
        connect(m_html, SIGNAL(completed()), this, SLOT(slotCompleted()));
        m_html->setJScriptEnabled(false);
        m_html->setJavaEnabled(false);
        m_html->setPluginsEnabled(false);
        m_html->setMetaRefreshEnabled(false);
        m_html->setOnlyLocalReferences(true);
    }

    QFile file( path );

    if( !file.open( IO_ReadOnly ) )
         return false;

    QTextStream stream( &file );
    CP437Codec codec;
    stream.setCodec( &codec );
    QString text( "<html><pre>");

    while( !stream.atEnd() ){
        text += stream.readLine() + "<br>";
    }

    text += "<pre><html>";
    file.close();
    m_html->write( text );
    int t = startTimer(5000);
    qApp->enter_loop();
    killTimer(t);

    // render the HTML page on a bigger pixmap and use smoothScale,
    // looks better than directly scaling with the QPainter (malte)
    QPixmap pix;
    if (width > 400 || height > 600)
    {
        if (height * 3 > width * 4)
            pix.resize(width, width * 4 / 3);
        else
            pix.resize(height * 3 / 4, height);
    }
    else
        pix.resize(400, 600);

    // light-grey background, in case loadind the page failed
    pix.fill( QColor( 245, 245, 245 ) );

    int borderX = pix.width() / width, borderY = pix.height() / height;
    QRect rc(borderX, borderY, pix.width() - borderX * 2, pix.height() - borderY * 2);
    QPainter p;

    p.begin(&pix);
    m_html->paint(&p, rc);
    p.end();

    img = pix.convertToImage();
    return true;
}

void KNfoViewerPreview::timerEvent(QTimerEvent *)
{
    qApp->exit_loop();
}

void KNfoViewerPreview::slotCompleted()
{
    qApp->exit_loop();
}

ThumbCreator::Flags KNfoViewerPreview::flags() const
{
    return DrawFrame;
}

#include "knfoviewerpreview.moc"
