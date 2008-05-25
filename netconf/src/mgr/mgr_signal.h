#ifndef _H_mgr_signal
#define _H_mgr_signal

/*  FILE: mgr_signal.h
*********************************************************************
*								    *
*			 P U R P O S E				    *
*								    *
*********************************************************************

    Handle interrupt signals for the manager


*********************************************************************
*								    *
*		   C H A N G E	 H I S T O R Y			    *
*								    *
*********************************************************************

date	     init     comment
----------------------------------------------------------------------
20-feb-07    abb      Begun

*/

#ifndef _H_status
#include "status.h"
#endif


/* don't rely on GNU extension being defined */
typedef void (*sighandler_t)(int signum);

/********************************************************************
*								    *
*			F U N C T I O N S			    *
*								    *
*********************************************************************/

extern void
    mgr_signal_init (void);

extern void 
    mgr_signal_cleanup (void);

extern void
    mgr_signal_handler (int intr);

extern void 
    mgr_signal_install_break_handler (sighandler_t handler);

#endif	    /* _H_mgr_signal */
