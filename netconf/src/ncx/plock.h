/*
 * Copyright (c) 2010 Netconf Central, Inc.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.    
 */
#ifndef _H_plock
#define _H_plock
/*  FILE: plock.h
*********************************************************************
*                                                                   *
*                         P U R P O S E                             *
*                                                                   *
*********************************************************************

    RFC 57517 partial lock support


*********************************************************************
*                                                                   *
*                   C H A N G E         H I S T O R Y               *
*                                                                   *
*********************************************************************

date             init     comment
----------------------------------------------------------------------
21-jun-10    abb      Begun.

*/

#ifndef _H_dlq
#include "dlq.h"
#endif

#ifndef _H_status
#include "status.h"
#endif

#ifndef _H_tstamp
#include "tstamp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************
*                                                                   *
*                         C O N S T A N T S                         *
*                                                                   *
*********************************************************************/



/********************************************************************
*                                                                   *
*                        T Y P E S                                  *
*                                                                   *
*********************************************************************/

/* matches lock-id-type in YANG module */
typedef uint32 plock_id_t;

/* struct representing 1 configuration database */
typedef struct plock_cb_t_ {
    dlq_hdr_t       qhdr;
    plock_id_t      plock_id;
    uint32          plock_sesid;
    struct xpath_pcb_t_    *plock_pcb;
    struct xpath_result_t_ *plock_result;
    xmlChar         plock_time[TSTAMP_MIN_SIZE];
} plock_cb_t;


/********************************************************************
*                                                                   *
*                        F U N C T I O N S                          *
*                                                                   *
*********************************************************************/


/********************************************************************
* FUNCTION plock_new_cb
*
* Create a new partial lock control block
*
* INPUTS:
*   res == address of return status
*
* OUTPUTS:
*   *res == return status
*
* RETURNS:
*   pointer to initialized PLCB, or NULL if some error
*   this struct must be freed by the caller
*********************************************************************/
extern plock_cb_t *
    plock_new_cb (status_t *res);


/********************************************************************
* FUNCTION plock_free_cb
*
* Free a partial lock control block
*
* INPUTS:
*   plcb == partial lock control block to free
*
*********************************************************************/
extern void
    plock_free_cb (plock_cb_t *plcb);


#ifdef __cplusplus
}  /* end extern 'C' */
#endif


/********************************************************************
* FUNCTION plock_reset_id
*
* Set the next ID number back to the start
* Only the caller maintaining a queue of plcb
* can decide if the ID should rollover
*
*********************************************************************/
extern void
    plock_reset_id (void);


/********************************************************************
* FUNCTION plock_get_id
*
* Get the lock ID for this partial lock
*
* INPUTS:
*   plcb == partial lock control block to use
*
*********************************************************************/
extern plock_id_t
    plock_get_id (plock_cb_t *plcb);


/********************************************************************
* FUNCTION plock_get_sid
*
* Get the session ID holding this partial lock
*
* INPUTS:
*   plcb == partial lock control block to use
*
*********************************************************************/
extern uint32
    plock_get_sid (plock_cb_t *plcb);


#endif            /* _H_plock */
