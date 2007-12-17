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
#ifndef _CP437CODEC_H_
#define _CP437CODEC_H_

//Avoid QString mangling
#if !defined( QT_NO_ASCII_CAST )
#define QT_NO_ASCII_CAST 1
#endif

#ifndef QT_H
#include <QTextCodec>
#endif // QT_H

#ifndef QT_NO_CODECS

class CP437Codec4 : public QTextCodec
{
    public:
        CP437Codec4();
        ~CP437Codec4();
        int mibEnum() const;
        QByteArray name() const;
        QList<QByteArray> aliases () const;

#if !defined(Q_NO_USING_KEYWORD)
        using QTextCodec::fromUnicode;
#endif
        QByteArray convertFromUnicode( const QChar *input, int number, ConverterState *state = 0 ) const;
        QString convertToUnicode ( const char * chars, int len, ConverterState * state = 0 ) const;
        int heuristicContentMatch(const char* chars, int len) const;
        int heuristicNameMatch(const char* hint) const;
};

#endif // QT_NO_CODECS

#endif // _CP437CODEC_H_
