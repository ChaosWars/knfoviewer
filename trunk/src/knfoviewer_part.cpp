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
#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <klocale.h>
#include <kio/netaccess.h>
#include <qtextedit.h>
#include <qfile.h>
#include <qregexp.h>
#include <qsimplerichtext.h>
#include "knfoviewer_part.h"
#include "cp437codec.h"
#include "knfoviewersettings.h"

KNfoViewerPart::KNfoViewerPart( QWidget *parentWidget, const char *widgetName,
                              QObject *parent, const char *name )
    : KParts::ReadOnlyPart(parent, name)
{
    // we need an instance
    setInstance( KNfoViewerPartFactory::instance() );

    // this should be your custom internal widget
    m_widget = new QTextEdit( parentWidget );
    m_widget->setWordWrap( QTextEdit::NoWrap );
    m_widget->setTextFormat( QTextEdit::RichText );
    m_widget->setReadOnly( true );

    // notify the part that this is our internal widget
    setWidget( m_widget );

    // create our actions
    KStdAction::open( this, SLOT( fileOpen() ), actionCollection() );
    config = KNfoViewerSettings::self();
    readProperties( config );

    // set our XML-UI resource file
    setXMLFile( "knfoviewer_part.rc" );
}

KNfoViewerPart::~KNfoViewerPart()
{
    saveProperties( config );
}

void KNfoViewerPart::saveProperties( KNfoViewerSettings *config )
{
    config->setFont( m_widget->font().toString() );
    config->writeConfig();
}

void KNfoViewerPart::readProperties( KNfoViewerSettings *config )
{
    QFont font;
    font.fromString( config->font() );
    m_widget->setFont( font );
}

void KNfoViewerPart::setBrowserFont( const QFont &font )
{
    m_widget->setFont( font );
}

bool KNfoViewerPart::openFile()
{
    // m_file is always local so we can use QFile on it
    QFile file( m_file );

    if( !file.open( IO_ReadOnly ) )
        return false;

    QString str;
    QTextStream stream( &file );
    CP437Codec codec;
    stream.setCodec( &codec );
    QRegExp exp( "http://*" );

    while( !stream.atEnd() ){
        QString s = stream.readLine();
        int pos = 0;

        while( ( pos = s.find( exp, pos ) ) > -1 ){

            int end = pos + 7;
            QChar c( s.at( end ) );

            while( !c.isSpace() && c.category() != QChar::Separator_Line && end != s.length() ){
                end++;
                c = s.at( end );
            }

            QString l = s.mid( pos, end - pos );
            QString link( "<a href=\"" + l + "\">" + l + "</a>" );
            s.replace( pos, l );
            pos += link.length();
        }

            str += s + "<br>";
    }

    file.close();

    // now that we have the entire file, display it
    m_widget->setText( str );

    // just for fun, set the status bar
    emit setStatusBarText( m_url.prettyURL() );

    return true;
}

bool KNfoViewerPart::openURL( const KURL & url )
{
    emit setWindowCaption( url.prettyURL() );
    emit addRecentFile( url );
    m_file = KIO::NetAccess::mostLocalURL( url, 0 ).path();
    return openFile();
}

void KNfoViewerPart::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    QString file_name = KFileDialog::getOpenFileName( QDir::homeDirPath(), "*.nfo *.NFO | NFO Files", 0, NULL );

    if (file_name.isEmpty() == false){
        openURL( file_name );
    }
}

// It's usually safe to leave the factory code alone.. with the
// notable exception of the KAboutData data
#include <kaboutdata.h>
#include <klocale.h>

    KInstance*  KNfoViewerPartFactory::s_instance = 0L;
    KAboutData* KNfoViewerPartFactory::s_about = 0L;

    KNfoViewerPartFactory::KNfoViewerPartFactory()
        : KParts::Factory()
{
}

KNfoViewerPartFactory::~KNfoViewerPartFactory()
{
    delete s_instance;
    delete s_about;

    s_instance = 0L;
}

KParts::Part* KNfoViewerPartFactory::createPartObject(   QWidget *parentWidget, const char *widgetName,
                                                    QObject *parent, const char *name,
                                                    const char *classname, const QStringList &args )
{
    // Create an instance of our Part
    KNfoViewerPart* obj = new KNfoViewerPart( parentWidget, widgetName, parent, name );
    return obj;
}

KInstance* KNfoViewerPartFactory::instance()
{
    if( !s_instance )
    {
        s_about = new KAboutData("knfoviewer", I18N_NOOP("KNfoViewer"), "0.1");
        s_about->addAuthor("Lawrence Lee", 0, "valher@facticius.net");
        s_instance = new KInstance(s_about);
    }
    return s_instance;
}

extern "C"
{
    void* init_libknfoviewerpart()
    {
        KGlobal::locale()->insertCatalogue("knfoviewer");
        return new KNfoViewerPartFactory;
    }
};

#include "knfoviewer_part.moc"
