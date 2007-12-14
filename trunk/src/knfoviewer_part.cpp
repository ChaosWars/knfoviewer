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
#include <khtmlview.h>
#include <qfile.h>
#include <qregexp.h>
#include <qlayout.h>
#include "knfoviewer_part.h"
#include "knfoviewerhtml.h"
#include "cp437codec.h"
#include "knfoviewersettings.h"
#include "settings.h"

KNfoViewerPart::KNfoViewerPart( QWidget *parentWidget, const char *widgetName,
                              QObject *parent, const char *name )
    : KParts::ReadOnlyPart( parent, name )
{
    // we need an instance
    setInstance( KNfoViewerPartFactory::instance() );

    // this should be your custom internal widget
    m_widget = new QWidget( parentWidget );
    layout = new QGridLayout( m_widget );
    htmlpart = new KNfoViewerHTML( m_widget );
    layout->addWidget( htmlpart->view(), 0, 0, 0 );
    htmlpart->setZoomFactor( 100 );
    htmlpart->setJScriptEnabled(false);
    htmlpart->setJavaEnabled(false);
    htmlpart->setMetaRefreshEnabled(false);
    htmlpart->setPluginsEnabled(false);
    htmlpart->setOnlyLocalReferences(true);

    // notify the part that this is our internal widget
    setWidget( m_widget );

    // create our actions
    KStdAction::open( this, SLOT( fileOpen() ), actionCollection() );
    (void) new KAction( i18n( "&Configure KNfoViewer" ), 0, this, SLOT( optionsConfigure() ),
                                            actionCollection(), "options_configure" );
    config = KNfoViewerSettings::self();
    readProperties( config );

    // set our XML-UI resource file
    setXMLFile( "knfoviewer_part.rc" );

    display();
}

KNfoViewerPart::~KNfoViewerPart()
{
    saveProperties( config );
    delete htmlpart;
    delete layout;
}

void KNfoViewerPart::saveProperties( KNfoViewerSettings *config )
{
    config->setFont( font.toString() );
    config->setBackgroundColor( backgroundColor.rgb() );
    config->setTextColor( textColor.rgb() );
    config->setLinkColor( linkColor.rgb() );
    config->writeConfig();
}

void KNfoViewerPart::readProperties( KNfoViewerSettings *config )
{
    font.fromString( config->font() );
    backgroundColor = config->backgroundColor();
    textColor = config->textColor();
    linkColor = config->linkColor();
}

void KNfoViewerPart::optionsConfigure()
{
    if( KConfigDialog::showDialog( "settings" ) )
        return;

    settings = new Settings( m_widget, "settings", config );
    connect( settings, SIGNAL( settingsChanged() ), this, SLOT( loadSettings() ) );
    settings->show();
}

void KNfoViewerPart::loadSettings()
{
    readProperties( config );
    display();
}

bool KNfoViewerPart::openFile()
{
    // m_file is always local so we can use QFile on it
    QFile file( m_file );

    if( !file.open( IO_ReadOnly ) )
        return false;

    text = "";
    QTextStream stream( &file );
    CP437Codec codec;
    stream.setCodec( &codec );
    QString s;

    while( !stream.atEnd() ){
        s = stream.readLine();

        //Examine the text for hyperlinks
        QRegExp exp( "http://*" );
        int pos = 0;
        QChar c;

        while( ( pos = s.find( exp, pos ) ) > -1 ){
            int end = pos + 7;
            c = s.at( end );

            while( !c.isSpace() && c.category() != QChar::Separator_Line && end != s.length() ){
                end++;
                c = s.at( end );
            }

            QString l = s.mid( pos, end - pos );
            QString link( "<a href=\"" + l + "\">" + l + "</a>" );
            s.replace( pos, l.length(), link );
            pos += link.length();
        }

        text += s + "\n";
    }

    file.close();

    // now that we have the entire file, display it
    display();

    // just for fun, set the status bar
    emit setStatusBarText( m_url.prettyURL() );

    return true;
}

const QString KNfoViewerPart::htmlCode( const QString &text )
{
    int fontSize = font.pointSize();
    QString code;
    QString bc;
    bc.sprintf( "%x", backgroundColor.rgb() );
    bc = bc.right( 6 );
    QString tc;
    tc.sprintf( "%x", textColor.rgb() );
    tc = tc.right( 6 );
    QString lc;
    lc.sprintf( "%x", linkColor.rgb() );
    lc = lc.right( 6 );
    code = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n\
            <html>\n\
            <head>\n\
            <meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\" />\n\
            <style type=\"text/css\" media=\"screen\"><!--\n\
            body {\n";
    code +=     "color : #" + tc + ";\n";
    code +=     "background-color: #" + bc + ";\n";
    code +=     "margin: 0px;\n\
            }\n\
            #nfo {\n";
    code +=     "color: #" + bc + ";\n";
    code +=     "background-color: transparent;\n\
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
            #data {\n";
    code +=     "font-size: " + QString::number( fontSize ) + "px;\n";
    code +=     "font-family: \"" + font.family() + "\";\n";
    code +=     "line-height: " + QString::number( fontSize ) + "px;\n";
    code +=     "background-color: #" + bc + ";\n";
    code +=     "color: #" + tc + ";\n";
    code +=     "position: relative;\n\
                white-space: pre;\n\
                visibility : visible;\n";
    code += "}\n\
            a {\n";
    code +=     "color: #" + lc + ";\n";
    code +=     "text-decoration: none;\n\
            }\n\
            a:hover {\n";
    code +=     "color: #" + lc + ";\n";
    code +=     "text-decoration: none;\n\
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

void KNfoViewerPart::display()
{
    htmlpart->begin();
    htmlpart->write( htmlCode( text ) );
    htmlpart->end();
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
        s_about = new KAboutData("knfoviewer", I18N_NOOP("KNfoViewer"), "0.4");
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
