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
#include <KDE/KStandardDirs>
#include <KDE/KApplication>
#include <KDE/KMimeType>
#include <KDE/KHTMLPart>
#include <KDE/KHTMLView>
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QFile>
#include "knfoviewerthumbnail.h"
#include "cp437codec.h"

extern "C"
{
    ThumbCreator *new_creator()
    {
        return new KNFOViewerThumbnail();
    }
}

/*
 * This code is taken nearly verbatim from htmlcreator.cpp
 * in kdebase/runtime/kioslave/thumbnail/ with minor changes to
 * ensure the correct rendering of CP437 encoded characters.
 */
KNFOViewerThumbnail::KNFOViewerThumbnail() : m_html( 0 )
{
}

KNFOViewerThumbnail::~KNFOViewerThumbnail()
{
    delete m_html;
}

bool KNFOViewerThumbnail::create( const QString &path, int width, int height, QImage &img )
{
    // Filter out unwanted mimetypes
//     KMimeType::Ptr mimeType = KMimeType::findByPath( path );
//
//     if( !mimeType->is( "text/x-nfo" ) )
//         return false;

    if( !m_html ){
        m_html = new KHTMLPart();
        connect( m_html, SIGNAL( completed() ), SLOT( slotCompleted() ) );
        m_html->setJScriptEnabled(false);
        m_html->setJavaEnabled(false);
        m_html->setPluginsEnabled(false);
        m_html->setMetaRefreshEnabled(false);
        m_html->setOnlyLocalReferences(true);
    }

    QFile file( path );

    if( !file.open( QIODevice::ReadOnly ) )
         return false;

    QString text;
    QTextStream stream( &file );
    CP437Codec codec;
    stream.setCodec( &codec );

    while( !stream.atEnd() ){
        text += stream.readLine() + "\n";
    }

    m_html->begin();
    m_html->write( htmlCode( text ) );
    m_html->end();
    m_html->view()->resize( 400, 600 );

    int t = startTimer(5000);
    m_eventLoop.exec( QEventLoop::ExcludeUserInputEvents );
    killTimer(t);

    // render the HTML page on a bigger pixmap and use smoothScale,
    // looks better than directly scaling with the QPainter (malte)
    QPixmap pix;
    if (width > 400 || height > 600)
    {
        if (height * 3 > width * 4)
            pix = QPixmap(width, width * 4 / 3);
        else
            pix = QPixmap(height * 3 / 4, height);
    }
    else
        pix = QPixmap(400, 600);

    // light-grey background, in case loadind the page failed
    pix.fill( QColor( 245, 245, 245 ) );

    int borderX = pix.width() / width, borderY = pix.height() / height;
    QRect rc(borderX, borderY, pix.width() - borderX * 2, pix.height() - borderY * 2);

    QPainter p;
    p.begin(&pix);
    m_html->paint(&p, rc);
    p.end();

    img = pix.toImage();
    file.close();
    return true;
}

const QString KNFOViewerThumbnail::htmlCode( const QString &text )
{
    QString code = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n\
            <html>\n\
            <head>\n\
            <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\" />\n\
            <style type=\"text/css\" media=\"screen\"><!--\n\
            body {\n \
                color : #000000;\n\
                background-color: #ffffff;\n\
                margin: 0px;\n\
            }\n\
            #nfo {\n\
                color: #000000;\n\
                background-color: transparent;\n\
                text-align: center;\n\
                position: absolute;\n\
                top: 0px;\n\
                left: 0px;\n\
                width: 100%;\n\
                height: 100%;\n\
                overflow: visible;\n\
                visibility: visible;\n\
                display: block\n\
            }\n\
            #data {\n\
                font-size: 11px;\n\
                font-family: \"Andale Mono\";\n\
                line-height: 11px;\n\
                background-color: #ffffff;\n\
                color: #000000;\n\
                position: relative;\n\
                white-space: pre;\n\
                visibility : visible;\n\
            }\n\
            a {\n\
                color: #000000;\n\
                text-decoration: none;\n\
            }\n\
            a:hover {\n\
                color: #000000;\n\
                text-decoration: none;\n\
}\n\
            --></style>\n\
            </head>\n\
            <body>\n\
            <div id\"nfo\">\n\
            <div id=\"data\">\n";

    code += text;
    code += "<br></div></div><br/></body></html>";

    return code;
}

void KNFOViewerThumbnail::timerEvent(QTimerEvent *)
{
    m_eventLoop.quit();
}

void KNFOViewerThumbnail::slotCompleted()
{
    m_eventLoop.quit();
}

ThumbCreator::Flags KNFOViewerThumbnail::flags() const
{
    return DrawFrame;
}

#include "knfoviewerthumbnail.moc"
