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
#include "cp437codec.h"

#ifndef QT_NO_CODECS

//#define IsCP437Char(c)  ( ( ( (c) <= 0x1F ) && ( (c) != 0x0A ) && ( (c) != 0x0D ) ) || ( ( (c) >= 0x7F ) && ( (c) <= 0xFF ) ) )
#define IsCP437Char(c)  ( ( (c) >= 0x80 ) && ( (c) <= 0xFF ) )
#define QValidChar(u)   ( (u) ? QChar( (u) ) : QChar::replacement )

CP437Codec::CP437Codec()
{
}

CP437Codec::~CP437Codec()
{
}

int CP437Codec::mibEnum() const
{
    return 2011;
}

const char* CP437Codec::name() const
{
    return "CP437";
}

// static const ushort UnToCP437[][2] = {
// };

static const ushort CP437ToUn[][2] = {
//     {0x0001,0x263A},
//     {0x0002,0x263B},
//     {0x0003,0x2665},
//     {0x0004,0x2666},
//     {0x0005,0x2663},
//     {0x0006,0x2660},
//     {0x0007,0x2022},
//     {0x0008,0x25D8},
//     {0x0009,0x25CB},
//     {0x000A,0x25D9},
//     {0x000B,0x2642},
//     {0x000C,0x2640},
//     {0x000D,0x266A},
//     {0x000E,0x266B},
//     {0x000F,0x263C},
//     {0x0010,0x25BA},
//     {0x0011,0x25C4},
//     {0x0012,0x2195},
//     {0x0013,0x203C},
//     {0x0014,0x00B6},
//     {0x0015,0x00A7},
//     {0x0016,0x21AC},
//     {0x0017,0x21A8},
//     {0x0018,0x2191},
//     {0x0019,0x2193},
//     {0x001A,0x2192},
//     {0x001B,0x2190},
//     {0x001C,0x221F},
//     {0x001D,0x2194},
//     {0x001E,0x25B2},
//     {0x001F,0x25BC},
//     {0x007F,0x2302},
    {0x0080,0x00C7},
    {0x0081,0x00FC},
    {0x0082,0x00E9},
    {0x0083,0x00E2},
    {0x0084,0x00E4},
    {0x0085,0x00E0},
    {0x0086,0x00E5},
    {0x0087,0x00E7},
    {0x0088,0x00EA},
    {0x0089,0x00EB},
    {0x008A,0x00E8},
    {0x008B,0x00EF},
    {0x008C,0x00EE},
    {0x008D,0x00EC},
    {0x008E,0x00C4},
    {0x008F,0x00C5},
    {0x0090,0x00C9},
    {0x0091,0x00E6},
    {0x0092,0x00C6},
    {0x0093,0x00F4},
    {0x0094,0x00F6},
    {0x0095,0x00F2},
    {0x0096,0x00FB},
    {0x0097,0x00F9},
    {0x0098,0x00FF},
    {0x0099,0x00D6},
    {0x009A,0x00DC},
    {0x009B,0x00A2},
    {0x009C,0x00A3},
    {0x009D,0x00D5},
    {0x009E,0x20A7},
    {0x009F,0x0192},
    {0x00A0,0x00E1},
    {0x00A1,0x00ED},
    {0x00A2,0x00F3},
    {0x00A3,0x00FA},
    {0x00A4,0x00F1},
    {0x00A5,0x00D1},
    {0x00A6,0x00AA},
    {0x00A7,0x00BA},
    {0x00A8,0x00BF},
    {0x00A9,0x2310},
    {0x00AA,0x00AC},
    {0x00AB,0x00BD},
    {0x00AC,0x00BC},
    {0x00AD,0x00A1},
    {0x00AE,0x00AB},
    {0x00AF,0x00BB},
    {0x00B0,0x2591},
    {0x00B1,0x2592},
    {0x00B2,0x2593},
    {0x00B3,0x2502},
    {0x00B4,0x2524},
    {0x00B5,0x2561},
    {0x00B6,0x2562},
    {0x00B7,0x2556},
    {0x00B8,0x2555},
    {0x00B9,0x2563},
    {0x00BA,0x2551},
    {0x00BB,0x2557},
    {0x00BC,0x255D},
    {0x00BD,0x255C},
    {0x00BE,0x255B},
    {0x00BF,0x2510},
    {0x00C0,0x2514},
    {0x00C1,0x2534},
    {0x00C2,0x252C},
    {0x00C3,0x251C},
    {0x00C4,0x2500},
    {0x00C5,0x253C},
    {0x00C6,0x255E},
    {0x00C7,0x255F},
    {0x00C8,0x255A},
    {0x00C9,0x2554},
    {0x00CA,0x2569},
    {0x00CB,0x2566},
    {0x00CC,0x2560},
    {0x00CD,0x2550},
    {0x00CE,0x256C},
    {0x00CF,0x2567},
    {0x00D0,0x2568},
    {0x00D1,0x2564},
    {0x00D2,0x2565},
    {0x00D3,0x2559},
    {0x00D4,0x2558},
    {0x00D5,0x2552},
    {0x00D6,0x2553},
    {0x00D7,0x256B},
    {0x00D8,0x256A},
    {0x00D9,0x2518},
    {0x00DA,0x250C},
    {0x00DB,0x2588},
    {0x00DC,0x2584},
    {0x00DD,0x258C},
    {0x00DE,0x2590},
    {0x00DF,0x2580},
    {0x00E0,0x03B1},
    {0x00E1,0x00DF},
    {0x00E2,0x0393},
    {0x00E3,0x03C0},
    {0x00E4,0x03A3},
    {0x00E5,0x00C3},
    {0x00E6,0x00B5},
    {0x00E7,0x03C4},
    {0x00E8,0x03A6},
    {0x00E9,0x0398},
    {0x00EA,0x03A9},
    {0x00EB,0x03B4},
    {0x00EC,0x221E},
    {0x00ED,0x03C6},
    {0x00EE,0x03B5},
    {0x00EF,0x2229},
    {0x00F0,0x2261},
    {0x00F1,0x00B1},
    {0x00F2,0x2265},
    {0x00F3,0x2264},
    {0x00F4,0x2320},
    {0x00F5,0x2321},
    {0x00F6,0x00F7},
    {0x00F7,0x2248},
    {0x00F8,0x00B0},
    {0x00F9,0x2219},
    {0x00FA,0x00B7},
    {0x00FB,0x221A},
    {0x00FC,0x207F},
    {0x00FD,0x00B2},
    {0x00FE,0x25A0},
    {0x00FF,0x00A0}
};

QCString CP437Codec::fromUnicode( const QString& uc, int& /*lenInOut*/ ) const
{
    return uc.utf8();
}

QString CP437Codec::toUnicode( const char *chars, int len ) const
{
    QString result;

    for ( int i = 0; i < len; i++ ) {
        uchar ch = chars[i];

        if (/*( ch == 0x00 ) || ( ch == 0x0A ) || ( ch == 0x0D ) || ( ch >= 0x20 &&*/ ch <= 0x7E ) {
            //ASCII
            result += QChar( ch );

        } else if ( IsCP437Char( ch ) ) {
            //CP437
//             if ( ch <= 0x1F ) {
//                 result += QValidChar( CP437ToUn[ch - 0x01][1] );
//
//             } else {
                result += QValidChar( CP437ToUn[ch - 0x80][1] );
//             }

        } else {
            //Invalid charachter
            result += QChar::replacement;
        }

    }

    return result;
}

QChar CP437Codec::charToUnicode( const uchar ch ) const
{
    QChar result;

    if (/*( ch == 0x00 ) || ( ch == 0x0A ) || ( ch == 0x0D ) || ( ch >= 0x20 &&*/ ch <= 0x7E ) {
            //ASCII
        result = QChar( ch );

    } else if ( IsCP437Char( ch ) ) {
            //CP437
//             if ( ch <= 0x1F ) {
//                 result += QValidChar( CP437ToUn[ch - 0x01][1] );
        //
//             } else {
        result = QValidChar( CP437ToUn[ch - 0x80][1] );
//             }

    } else {
            //Invalid charachter
        result = QChar::replacement;
    }

    return result;
}

int CP437Codec::heuristicContentMatch( const char *chars, int len ) const
{
    int score = 0;

    for ( int i=0; i<len; i++ ) {
        uchar ch = chars[i];
        // No nulls allowed.

        if ( !ch )
            return -1;

        if ( ch < 32 && ch != '\t' && ch != '\n' && ch != '\r' ) {
            // Suspicious
            if ( score )
                score--;

        } else if ( ch < 0x80 ) {
            // Inconclusive
        } else if ( IsCP437Char( ch ) ) {
            score++;

        } else {
            // Invalid
            return -1;
        }
    }

    return score;
}

int CP437Codec::heuristicNameMatch( const char *hint ) const
{
    const char *p = strchr( hint, '.' );

    if ( p )
        p++;
    else
        p = hint;

    if ( qstricmp( p, "IBM437" ) == 0 ||
            qstricmp( p, "CP437" ) == 0 )
        return 4;

    return QTextCodec::heuristicNameMatch( hint );
}

#endif