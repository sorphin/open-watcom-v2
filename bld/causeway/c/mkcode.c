/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Convertor from binary format file to C source code, for
*                assembly code bursts.
*
****************************************************************************/


/* This program is used to link against an assembly file containing
   code bursts for inline to C source.
   This allows us to use an assembler to translate the code bursts into
   the appropriate byte sequences. This program convert binary file
   generated by assembler and linker to appropriate C source code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFSIZE         4096

int main(int argc, char *argv[])
{
    FILE            *fp;
    FILE            *fi;
    int             i;
    size_t          len;
    unsigned char   *buff;
    unsigned char   *p;
    size_t          size;
    int             rc;

    if( argc < 3 ) {
        printf( "Usage: inp.file out.file\n" );
        return( 1 );
    }
    rc = 1;
    buff = malloc( BUFSIZE );
    if( buff == NULL ) {
        printf( "Error: Not enough memory.\n" );
    } else {
        fi = fopen( argv[1], "rb" );
        if( fi == NULL ) {
            printf( "Error: Can not open input file.\n" );
        } else {
            fp = fopen( argv[2], "w" );
            if( fp == NULL ) {
                printf( "Error: Can not create output file.\n" );
            } else {
                i = 0;
                fprintf( fp, "    " );
                for( ; ferror( fi ) == 0; ) {
                    size = fread( buff, 1, BUFSIZE, fi );
                    if( size == 0 ) {
                        break;
                    }
                    p = buff;
                    for( len = size; len > 0; --len ) {
                        fprintf( fp, "0x%2.2X,", *p++ );
                        i++;
                        if( i == 16 ) {
                            fprintf( fp, "\n    " );
                            i = 0;
                        }
                    }
                }
                fprintf( fp, "\n" );
                fclose( fp );
                rc = 0;
            }
            fclose( fi );
        }
        free( buff );
    }
    return( rc );
}
