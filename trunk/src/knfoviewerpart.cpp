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

#include <KDE/KAboutData>
#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KConfigDialog>
#include <KDE/KFileDialog>
#include <KDE/KGlobal>
#include <KDE/KHTMLView>
#include <KDE/KIcon>
#include <KDE/KLocale>
#include <KDE/KStandardAction>
// #include <KDE/KTextBrowser>
#include <KDE/KIO/NetAccess>
#include <KDE/KParts/GenericFactory>
#include <QFile>
#include <QRegExp>
#include <QVBoxLayout>
// #include "colorpage.h"
#include "cp437codec.h"
// #include "fontpage.h"
#include "knfoviewerhtml.h"
#include "knfoviewerpart.h"
#include "knfoviewersettings.h"
#include "settingspage.h"

//Factory Code
typedef KParts::GenericFactory<KNFOViewerPart> KNFOViewerPartFactory;
K_EXPORT_PLUGIN( KNFOViewerPartFactory );
K_EXPORT_PLUGIN_VERSION( 0.4 );

KNFOViewerPart::KNFOViewerPart( QWidget *parentWidget, QObject *parent, const QStringList& )
    : KParts::ReadOnlyPart( parent ),
      font( KGlobalSettings::fixedFont() ),
      backgroundColor( QColor( 0, 0, 0 ) ),
      textColor( QColor( 128, 128, 128 ) ),
      linkColor( QColor( 0, 0, 255 ) )
{
    // this should be your custom internal widget
    m_widget = new QWidget( parentWidget );
    QVBoxLayout *layout = new QVBoxLayout( m_widget );
//     browser = new KTextBrowser( m_widget );
//     browser->setLineWrapMode( QTextEdit::NoWrap );
    htmlpart = new KNFOViewerHTML();
    layout->addWidget( htmlpart->view() );
//     layout->addWidget( browser );
    m_widget->setLayout( layout );
    htmlpart->setZoomFactor( 100 );
    htmlpart->setJScriptEnabled(false);
    htmlpart->setJavaEnabled(false);
    htmlpart->setMetaRefreshEnabled(false);
    htmlpart->setPluginsEnabled(false);
    htmlpart->setOnlyLocalReferences(true);
    // create our actions
    KStandardAction::open( this, SLOT( fileOpen() ), actionCollection() );
    KStandardAction::preferences( this, SLOT( optionsConfigure() ), actionCollection() );
//     connect( htmlpart, SIGNAL( onURL( const QString& ) ), this, SIGNAL( setStatusBarText( const QString& ) ) );
    // notify the part that this is our internal widget
    setWidget( m_widget );
    // set our XML-UI resource file
    setXMLFile( "knfoviewer/knfoviewerpart.rc" );
    loadSettings();
    display();
}

KNFOViewerPart::~KNFOViewerPart()
{
}

KAboutData* KNFOViewerPart::createAboutData()
{
    return new KAboutData( "knfoviewerpart", 0, ki18n( "KNFOViewer" ), "0.4.1" );
}

void KNFOViewerPart::optionsConfigure()
{
    if( KConfigDialog::showDialog( "settings" ) )
        return;

    KConfigDialog *configDialog = new KConfigDialog( m_widget, "settings", KNFOViewerSettings::self() );
    QWidget *settingsWidget = new QWidget();
    SettingsPage *settingsPage = new SettingsPage( settingsWidget );
    configDialog->addPage( settingsWidget, i18n( "General Settings" ), "preferences-desktop" );
//     QWidget *fontWidget = new QWidget();
//     new FontPage( fontWidget );
//     configDialog->addPage( fontWidget, i18n( "Font Settings" ), "preferences-desktop-font" );
//     QWidget *colorWidget = new QWidget();
//     new ColorPage( colorWidget );
//     configDialog->addPage( colorWidget, i18n( "Color Settings" ), "preferences-desktop-color" );
    connect( configDialog, SIGNAL( settingsChanged( QString ) ), this, SLOT( loadSettings() ) );
    configDialog->show();
}

void KNFOViewerPart::loadSettings()
{
    font = KNFOViewerSettings::fontRequester();
    backgroundColor = KNFOViewerSettings::backgroundColor();
    textColor = KNFOViewerSettings::textColor();
    linkColor = KNFOViewerSettings::linkColor();
    display();
}

bool KNFOViewerPart::openFile()
{
    return false;
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
//     browser->setHtml( htmlCode( text ) );
    htmlpart->begin();
    htmlpart->write( htmlCode( text ) );
    htmlpart->end();
}

bool KNFOViewerPart::openUrl( const KUrl & url )
{
    QString m_file( KIO::NetAccess::mostLocalUrl( url, 0 ).path() );
    // m_file is always local so we can use QFile on it
    QFile file(m_file );

    if( !file.open( QIODevice::ReadOnly ) )
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

        while( ( pos = s.indexOf( exp, pos ) ) > -1 ){
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
    emit setWindowCaption( url.prettyUrl() );
    emit addRecentFile( url );

    return true;
}

void KNFOViewerPart::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
    KUrl file_name = KFileDialog::getOpenUrl( KUrl(), "*.nfo *.NFO | NFO Files" );

    if( !file_name.isEmpty() ){
        openUrl( file_name );
    }
}

#include "knfoviewerpart.moc"
