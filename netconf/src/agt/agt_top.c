/*
 * Copyright (c) 2009, Andy Bierman
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.    
 */
/*  FILE: agt_top.c

  NCX Agent Top Element Handler

  This module uses a simple queue of top-level entries
  because there are not likely to be very many of them.

  Each top-level node is keyed by the owner name and 
  the element name.  
                
*********************************************************************
*                                                                   *
*                  C H A N G E   H I S T O R Y                      *
*                                                                   *
*********************************************************************

date         init     comment
----------------------------------------------------------------------
30dec05      abb      begun

*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <memory.h>

#include <xmlstring.h>
#include <xmlreader.h>

#ifndef _H_procdefs
#include  "procdefs.h"
#endif

#ifndef _H_agt
#include "agt.h"
#endif

#ifndef _H_agt_ses
#include "agt_ses.h"
#endif

#ifndef _H_agt_top
#include "agt_top.h"
#endif

#ifndef _H_agt_xml
#include "agt_xml.h"
#endif

#ifndef _H_dlq
#include "dlq.h"
#endif

#ifndef _H_log
#include "log.h"
#endif

#ifndef _H_ncxconst
#include "ncxconst.h"
#endif

#ifndef _H_ncx
#include "ncx.h"
#endif

#ifndef _H_status
#include  "status.h"
#endif

#ifndef _H_top
#include "top.h"
#endif

#ifndef _H_xmlns
#include "xmlns.h"
#endif

#ifndef _H_xml_util
#include "xml_util.h"
#endif

/********************************************************************
*                                                                   *
*                       C O N S T A N T S                           *
*                                                                   *
*********************************************************************/

#ifdef DEBUG
#define AGT_TOP_DEBUG 1
#endif

/********************************************************************
*                                                                   *
*                             T Y P E S                             *
*                                                                   *
*********************************************************************/

/********************************************************************
*                                                                   *
*                       V A R I A B L E S                            *
*                                                                   *
*********************************************************************/


/**************    E X T E R N A L   F U N C T I O N S **********/


/********************************************************************
* FUNCTION agt_top_dispatch_msg
* 
* Find the appropriate top node handler and call it
* called by the transport manager (through the session manager)
* when a new message is detected
*
* INPUTS:
*   scb == session control block containing the xmlreader
*          set at the start of an incoming message.
*
* RETURNS:
*  none
*********************************************************************/
void
    agt_top_dispatch_msg (ses_cb_t **ppscb)
{
    ses_total_stats_t  *myagttotals;
    agt_profile_t      *profile;
    xml_node_t          top;
    status_t            res;
    top_handler_t       handler;
    ses_cb_t           *scb = *ppscb;
    
#ifdef DEBUG
    if (!scb) {
        SET_ERROR(ERR_INTERNAL_PTR);
        return;
    }
#endif

    myagttotals = ses_get_total_stats();
    profile = agt_get_profile();

    xml_init_node(&top);

    /* get the first node */
    res = agt_xml_consume_node(scb, 
                               &top, 
                               NCX_LAYER_TRANSPORT, 
                               NULL);
    if (res != NO_ERR) {
        scb->stats.inBadRpcs++;
        myagttotals->stats.inBadRpcs++;
        myagttotals->droppedSessions++;

        if (LOGINFO) {
            log_info("\nagt_top: bad msg for session %d (%s)",
                     scb->sid, 
                     get_error_string(res));
        }

        xml_clean_node(&top);
        agt_ses_free_session(scb);

        /* set the supplied ptr to ptr to scb to NULL so that the 
         * caller of this function knows that it was deallotcated */
        *ppscb=NULL;
        return;
    }

#ifdef AGT_TOP_DEBUG
    if (LOGDEBUG3) {
        log_debug3("\nagt_top: got node");
        if (LOGDEBUG4 && scb->state != SES_ST_INIT) {
            xml_dump_node(&top);
        }
    }
#endif

    /* check node type and if handler exists, then call it */
    if (top.nodetyp==XML_NT_START || top.nodetyp==XML_NT_EMPTY) {
        /* find the owner, elname tuple in the topQ */
        handler = top_find_handler(top.module, top.elname);
        if (handler) {
            /* call the handler */
            (*handler)(scb, &top);
        } else {
            res = ERR_NCX_DEF_NOT_FOUND;
        }
    } else {
        res = ERR_NCX_WRONG_NODETYP;
    }

    /* check any error trying to invoke the top handler */
    if (res != NO_ERR) {
        scb->stats.inBadRpcs++;
        myagttotals->stats.inBadRpcs++;
        myagttotals->droppedSessions++;
        
        if (LOGINFO) {
            log_info("\nagt_top: bad msg for session %d (%s)",
                     scb->sid, 
                     get_error_string(res));
        }

        agt_ses_free_session(scb);

        /* set the supplied ptr to ptr to scb to NULL so that the 
         * caller of this function knows that it was deallotcated */
        *ppscb=NULL;

    } else if (profile->agt_stream_output &&
               scb->state == SES_ST_SHUTDOWN_REQ) {
        /* session was closed */
        agt_ses_kill_session(scb,
                             scb->killedbysid,
                             scb->termreason);
        /* set the supplied ptr to ptr to scb to NULL so that the 
         * caller of this function knows that it was deallotcated */
        *ppscb=NULL;
    }

    xml_clean_node(&top);

} /* agt_top_dispatch_msg */


/* END file agt_top.c */