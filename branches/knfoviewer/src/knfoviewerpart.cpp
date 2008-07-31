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
#include <kaction.h>
#include <kconfigdialog.h>
#include <kfiledialog.h>
#include <kglobal.h>
#include <khtmlview.h>
#include <kinstance.h>
#include <klocale.h>
#include <kstdaction.h>
#include <kio/netaccess.h>
#include <qfile.h>
#include <qlayout.h>
#include <qregexp.h>
#include "colorpage.h"
#include "cp437codec.h"
#include "fontpage.h"
#include "knfoviewerhtml.h"
#include "knfoviewerpart.h"
#include "knfoviewersettings.h"
#include "mainwidget.h"

KNFOViewerPart::KNFOViewerPart( QWidget *parentWidget, const char */*widgetName*/,
                                QObject *parent, const char *name )
    : KParts::ReadOnlyPart( parent, name ),
      font( KGlobalSettings::fixedFont() ),
      backgroundColor( QColor( 0, 0, 0 ) ),
      textColor( QColor( 128, 128, 128 ) ),
      linkColor( QColor( 0, 0, 255 ) )
{
    // we need an instance
    setInstance( KNFOViewerPartFactory::instance() );
    // this should be your custom internal widget
    m_widget = new MainWidget( parentWidget );
    layout = new QHBoxLayout( m_widget );
    htmlpart = new KNFOViewerHTML( m_widget );
    layout->addWidget( htmlpart->view() );
    htmlpart->setZoomFactor( 100 );
    htmlpart->setJScriptEnabled(false);
    htmlpart->setJavaEnabled(false);
    htmlpart->setMetaRefreshEnabled(false);
    htmlpart->setPluginsEnabled(false);
    htmlpart->setOnlyLocalReferences(true);
    // create our actions
    KStdAction::open( this, SLOT( fileOpen() ), actionCollection() );
    KStdAction::preferences( this, SLOT( optionsConfigure() ), actionCollection() );
    connect( htmlpart, SIGNAL( onURL( const QString& ) ), this, SIGNAL( setStatusBarText( const QString& ) ) );
    // notify the part that this is our internal widget
    setWidget( m_widget );
    // set our XML-UI resource file
    setXMLFile( "knfoviewerpart.rc" );
    loadSettings();
    display();
}

KNFOViewerPart::~KNFOViewerPart()
{
}

void KNFOViewerPart::optionsConfigure()
{
    if( KConfigDialog::showDialog( "settings" ) )
        return;

    KConfigDialog *configDialog = new KConfigDialog( m_widget, "settings", KNFOViewerSettings::self() );
    QWidget *fontWidget = new QWidget();
    QHBoxLayout *fontLayout = new QHBoxLayout( fontWidget );
    fontLayout->addWidget( new FontPage( fontWidget ) );
    configDialog->addPage( fontWidget, i18n( "Font Settings" ), "fonts" );
    QWidget *colorWidget = new QWidget();
    QHBoxLayout *colorLayout = new QHBoxLayout( colorWidget );
    colorLayout->addWidget( new ColorPage( colorWidget ) );
    configDialog->addPage( colorWidget, i18n( "Color Settings" ), "colorize" );
    connect( configDialog, SIGNAL( settingsChanged() ), this, SLOT( loadSettings() ) );
    configDialog->show();
}

void KNFOViewerPart::loadSettings()
{
    font = KNFOViewerSettings::fontChooser();
    backgroundColor = KNFOViewerSettings::backgroundColor();
    textColor = KNFOViewerSettings::textColor();
    linkColor = KNFOViewerSettings::linkColor();
    display();
}

bool KNFOViewerPart::openFile()
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

const QString KNFOViewerPart::htmlCode( const QString &text )
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

void KNFOViewerPart::display()
{
    htmlpart->begin();
    htmlpart->write( htmlCode( text ) );
    htmlpart->end();
}

bool KNFOViewerPart::openURL( const KURL & url )
{
    emit setWindowCaption( url.prettyURL() );
    emit addRecentFile( url );
    m_file = KIO::NetAccess::mostLocalURL( url, 0 ).path();
    return openFile();
}

void KNFOViewerPart::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    KURL file_name = KFileDialog::getOpenURL( QString::null, "*.nfo *.NFO | NFO Files" );

    if( !file_name.isEmpty() ){
        openURL( file_name );
    }
}

// It's usually safe to leave the factory code alone.. with the
// notable exception of the KAboutData data
#include <kaboutdata.h>
#include <klocale.h>

    KInstance*  KNFOViewerPartFactory::s_instance = 0L;
    KAboutData* KNFOViewerPartFactory::s_about = 0L;

    KNFOViewerPartFactory::KNFOViewerPartFactory()
        : KParts::Factory()
{
}

KNFOViewerPartFactory::~KNFOViewerPartFactory()
{
    delete s_instance;
    delete s_about;

    s_instance = 0L;
}

KParts::Part* KNFOViewerPartFactory::createPartObject( QWidget *parentWidget, const char *widgetName,
                                                       QObject *parent, const char *name,
                                                       const char */*classname*/, const QStringList &/*args*/ )
{
    // Create an instance of our Part
    KNFOViewerPart* obj = new KNFOViewerPart( parentWidget, widgetName, parent, name );
    return obj;
}

KInstance* KNFOViewerPartFactory::instance()
{
    if( !s_instance )
    {
        s_about = new KAboutData("knfoviewer", I18N_NOOP("KNFOViewer"), "0.3.4");
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
        return new KNFOViewerPartFactory;
    }
};

#include "knfoviewerpart.moc"
