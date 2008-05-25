#ifndef _H_rpc_err
#define _H_rpc_err
/*  FILE: rpc_err.h
*********************************************************************
*                                                                   *
*                         P U R P O S E                             *
*                                                                   *
*********************************************************************

    NETCONF protocol standard error definitions

*********************************************************************
*                                                                   *
*                   C H A N G E         H I S T O R Y               *
*                                                                   *
*********************************************************************

date             init     comment
----------------------------------------------------------------------
06-apr-05    abb      Begun.
*/

#ifndef _H_dlq
#include "dlq.h"
#endif

#ifndef _H_ncxtypes
#include "ncxtypes.h"
#endif

#ifndef _H_rpc
#include "rpc.h"
#endif

/********************************************************************
*                                                                   *
*                         C O N S T A N T S                         *
*                                                                   *
*********************************************************************/

/***

   From draft-ietf-netconf-prot-12.txt:

   Tag:         in-use
   Error-type:  protocol, application
   Severity:    error
   Error-info:  none
   Description: The request requires a resource that already in use.

   Tag:         invalid-value
   Error-type:  protocol, application
   Severity:    error
   Error-info:  none
   Description: The request specifies an unacceptable value for one
                or more parameters.

   Tag:         too-big
   Error-type:  transport, rpc, protocol, application
   Severity:    error
   Error-info:  none
   Description: The request or response (that would be generated) is too
                large for the implementation to handle.

   Tag:         missing-attribute
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-attribute> : name of the missing attribute
                <bad-element> : name of the element that should
                contain the missing attribute
   Description: An expected attribute is missing

   Tag:         bad-attribute
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-attribute> : name of the attribute w/ bad value
                <bad-element> : name of the element that contains
                the attribute with the bad value
   Description: An attribute value is not correct; e.g., wrong type,
                out of range, pattern mismatch

   Tag:         unknown-attribute
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-attribute> : name of the unexpected attribute
                <bad-element> : name of the element that contains
                the unexpected attribute
   Description: An unexpected attribute is present

   Tag:         missing-element
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-element> : name of the missing element
   Description: An expected element is missing

   Tag:         bad-element
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-element> : name of the element w/ bad value
   Description: An element value is not correct; e.g., wrong type,
                out of range, pattern mismatch

   Tag:         unknown-element
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-element> : name of the unexpected element
   Description: An unexpected element is present

   Tag:         unknown-namespace
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  <bad-element> : name of the element that contains
                the unexpected namespace
                <bad-namespace> : name of the unexpected namespace
   Description: An unexpected namespace is present

   Tag:         access-denied
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  none
   Description: Access to the requested RPC, protocol operation,
                or data model is denied because authorization failed

   Tag:         lock-denied
   Error-type:  protocol
   Severity:    error
   Error-info:  <session-id> : session ID of session holding the
                requested lock, or zero to indicate a non-NETCONF
                entity holds the lock
   Description: Access to the requested lock is denied because the
                lock is currently held by another entity

   Tag:         resource-denied
   Error-type:  transport, rpc, protocol, application
   Severity:    error
   Error-info:  none
   Description: Request could not be completed because of insufficient
                resources

   Tag:         rollback-failed
   Error-type:  protocol, application
   Severity:    error
   Error-info:  none
   Description: Request to rollback some configuration change (via
                rollback-on-error or discard-changes operations) was
                not completed for some reason.

   Tag:         data-exists
   Error-type:  application
   Severity:    error
   Error-info:  none
   Description: Request could not be completed because the relevant
                data model content already exists. For example,
                a 'create' operation was attempted on data which
                already exists.

   Tag:         data-missing
   Error-type:  application
   Severity:    error
   Error-info:  none
   Description: Request could not be completed because the relevant
                data model content does not exist.  For example,
                a 'replace' or 'delete' operation was attempted on
                data which does not exist.

   Tag:         operation-not-supported
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  none
   Description: Request could not be completed because the requested
                operation is not supported by this implementation.

   Tag:         operation-failed
   Error-type:  rpc, protocol, application
   Severity:    error
   Error-info:  none
   Description: Request could not be completed because the requested
                operation failed for some reason not covered by
                any other error condition.

   Tag:         partial-operation
   Error-type:  application
   Severity:    error
   Error-info:  <ok-element> : identifies an element in the data model
                for which the requested operation has been completed
                for that node and all its child nodes.  This element
                can appear zero or more times in the <error-info>
                container.

                <err-element> : identifies an element in the data model
                for which the requested operation has failed for that
                node and all its child nodes. This element
                can appear zero or more times in the <error-info>
                container.

                <noop-element> : identifies an element in the data model
                for which the requested operation was not attempted for
                that node and all its child nodes. This element
                can appear zero or more times in the <error-info>
                container.

   Description: Some part of the requested operation failed or was
                not attempted for some reason.  Full cleanup has
                not been performed (e.g., rollback not supported)
                by the server.  The error-info container is used
                to identify which portions of the application
                data model content for which the requested operation
                has succeeded (<ok-element>), failed (<bad-element>),
                or not attempted (<noop-element>).


***/

#ifndef _H_dlq
#include "dlq.h"
#endif

/********************************************************************
*                                                                   *
*                        C O N S T A N T S                          *
*                                                                   *
*********************************************************************/


/********************************************************************
*                                                                   *
*                          T Y P E S                                *
*                                                                   *
*********************************************************************/

/* enumerations for NETCONF standard errors */
typedef enum rpc_err_t_ {
    RPC_ERR_NONE,
    RPC_ERR_IN_USE,
    RPC_ERR_INVALID_VALUE,
    RPC_ERR_TOO_BIG,
    RPC_ERR_MISSING_ATTRIBUTE,
    RPC_ERR_BAD_ATTRIBUTE,
    RPC_ERR_UNKNOWN_ATTRIBUTE,
    RPC_ERR_MISSING_ELEMENT,
    RPC_ERR_BAD_ELEMENT,
    RPC_ERR_UNKNOWN_ELEMENT,
    RPC_ERR_UNKNOWN_NAMESPACE,
    RPC_ERR_ACCESS_DENIED,
    RPC_ERR_LOCK_DENIED,
    RPC_ERR_RESOURCE_DENIED,
    RPC_ERR_ROLLBACK_FAILED,
    RPC_ERR_DATA_EXISTS,
    RPC_ERR_DATA_MISSING,
    RPC_ERR_OPERATION_NOT_SUPPORTED,
    RPC_ERR_OPERATION_FAILED,
    RPC_ERR_PARTIAL_OPERATION
} rpc_err_t;


/* enumerations for NETCONF standard error severities */
typedef enum rpc_err_sev_t_ {
    RPC_ERR_SEV_NONE,
    RPC_ERR_SEV_WARNING,
    RPC_ERR_SEV_ERROR
} rpc_err_sev_t;

/* one error-info sub-element */
typedef struct rpc_err_info_ {
    dlq_hdr_t          qhdr;
    xmlns_id_t         name_nsid;
    const xmlChar     *name;
    xmlChar           *dname;
    boolean            isqname;     /* val_nsid + v.strval == QName */
    ncx_btype_t        val_btype;
    xmlns_id_t         val_nsid;
    xmlChar           *badns;      /* if val_nsid INVALID namespace */
    xmlChar           *dval;
    union {
	const xmlChar     *strval;     /* for string error content */
	ncx_num_t          numval;     /* for number error content */
	void              *cpxval;     /* val_value_t */
    } v;
} rpc_err_info_t;


typedef struct rpc_err_rec_t_ {
    dlq_hdr_t          error_qhdr;
    status_t           error_res;
    ncx_layer_t        error_type;
    rpc_err_t          error_id;
    rpc_err_sev_t      error_severity;
    const xmlChar     *error_tag;
    const xmlChar     *error_app_tag;
    xmlChar           *error_path;
    xmlChar           *error_message;
    const xmlChar     *error_message_lang;
    dlq_hdr_t          error_info;     /* Q of rpc_err_info_t */
} rpc_err_rec_t;


/********************************************************************
*                                                                   *
*                        F U N C T I O N S                          *
*                                                                   *
*********************************************************************/

extern const xmlChar *
    rpc_err_get_errtag (rpc_err_t errid);

extern rpc_err_rec_t *
    rpc_err_new_record (void);

extern void
    rpc_err_init_record (rpc_err_rec_t *err);

extern void
    rpc_err_free_record (rpc_err_rec_t *err);

extern void
    rpc_err_clean_record (rpc_err_rec_t *err);

extern rpc_err_info_t *
    rpc_err_new_info (void);

extern void
    rpc_err_free_info (rpc_err_info_t *errinfo);

extern void
    rpc_err_dump_errors (const rpc_msg_t  *msg);

extern const xmlChar *
    rpc_err_get_severity (rpc_err_sev_t  sev);

extern void 
    rpc_err_clean_errQ (dlq_hdr_t *errQ);

#endif            /* _H_rpc_err */
