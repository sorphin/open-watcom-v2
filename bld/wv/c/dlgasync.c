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
* Description:  Async run dialog
*
****************************************************************************/


#include "dbgdefn.h"
#include "dbgwind.h"
#include "guidlg.h"
#include "dbgerr.h"
#include "dbgtoggl.h"
#include "dlgasync.h"
#include "trapbrk.h"
#include "remasync.h"
#include "guitypes.h"


typedef struct dlg_async {
    unsigned cond;
} dlg_async;

static gui_window   *AsyncWnd = 0;
static dlg_async    dlg;

void AsyncNotify( void )
{
   if( AsyncWnd ) {
        dlg.cond = PollAsync();
        if( !( dlg.cond & COND_RUNNING ) ) {
#ifdef __RDOS__
            uisendescape();
#else
            GUICloseDialog( AsyncWnd );
            AsyncWnd = 0;
#endif            
        }
    }
}

OVL_EXTERN bool AsyncEvent( gui_window *gui, gui_event gui_ev, void *param )
{
    gui_ctl_id      id;

    switch( gui_ev ) {
    case GUI_INIT_DIALOG:
        dlg.cond = 0;
        GUISetFocus( gui, CTL_ASYNC_STOP );
        AsyncWnd = gui;
        return( true );
    case GUI_DIALOG_ESCAPE:
        AsyncWnd = 0;
        dlg.cond = StopAsync();
        return( true );
    case GUI_CONTROL_CLICKED :
        GUI_GETID( param, id );
        switch( id ) {
        case CTL_ASYNC_STOP:
            AsyncWnd = 0;
            dlg.cond = StopAsync();
            GUICloseDialog( gui );
            return( true );
        }
        return( false );
    case GUI_DESTROY:
        AsyncWnd = 0;
        return( true );
    }

    return( false );
}

extern unsigned DlgAsyncRun( void )
{
    ResDlgOpen( &AsyncEvent, 0, DIALOG_ASYNC_RUN );
    return( dlg.cond );
}
