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
#include <KDE/KApplication>
#include <KDE/KToolInvocation>
#include <QClipboard>
#include "knfoviewerhtml.h"

KNFOViewerHTML::KNFOViewerHTML()
{
    connect( this, SIGNAL( selectionChanged() ), this, SLOT( selectionSlot() ) );
}

KNFOViewerHTML::~KNFOViewerHTML()
{
}

bool KNFOViewerHTML::urlSelected( const QString &url, int /*button*/, int /*state*/, const QString&/*_target*/,
                                  const KParts::OpenUrlArguments&/*args*/, const KParts::BrowserArguments&/*browserArgs*/ )
{
    KToolInvocation::invokeBrowser( url );
    return true;
}

void KNFOViewerHTML::selectionSlot()
{
    kapp->clipboard()->setText( selectedText() );
}

#include "knfoviewerhtml.moc"
