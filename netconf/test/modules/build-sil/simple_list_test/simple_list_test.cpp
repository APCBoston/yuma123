
/* 

 * Copyright (c) 2009 - 2011, Andy Bierman
 * All Rights Reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *

*** Generated by yangdump 2.1.1440:1441

    Combined SIL module
    module simple_list_test
    revision 2008-11-20
    namespace http://netconfcentral.org/ns/simple_list_test

 */

#include <xmlstring.h>

#include "procdefs.h"
#include "agt.h"
#include "agt_cb.h"
#include "agt_timer.h"
#include "agt_util.h"
#include "dlq.h"
#include "ncx.h"
#include "ncxmod.h"
#include "ncxtypes.h"
#include "status.h"
#include "simple_list_test.h"

/* module static variables */
static ncx_module_t *simple_list_test_mod;
static obj_template_t *simple_list_obj;
static val_value_t *simple_list_val;

/* put your static variables here */


/********************************************************************
* FUNCTION y_simple_list_test_init_static_vars
* 
* initialize module static variables
* 
********************************************************************/
static void y_simple_list_test_init_static_vars (void)
{
    simple_list_test_mod = NULL;
    simple_list_obj = NULL;
    simple_list_val = NULL;

    /* init your static variables here */

} /* y_simple_list_test_init_static_vars */


/********************************************************************
* FUNCTION simple_list_test_simple_list_theList_theKey_edit
* 
* Edit database object callback
* Path: /simple_list/theList/theKey
* Add object instrumentation in COMMIT phase.
* 
* INPUTS:
*     see agt/agt_cb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t simple_list_test_simple_list_theList_theKey_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    val_value_t *errorval = (curval) ? curval : newval;

    if (LOGDEBUG) {
        log_debug("\nEnter simple_list_test_simple_list_theList_theKey_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }
    return res;

} /* simple_list_test_simple_list_theList_theKey_edit */


/********************************************************************
* FUNCTION simple_list_test_simple_list_theList_theVal_edit
* 
* Edit database object callback
* Path: /simple_list/theList/theVal
* Add object instrumentation in COMMIT phase.
* 
* INPUTS:
*     see agt/agt_cb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t simple_list_test_simple_list_theList_theVal_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    val_value_t *errorval = (curval) ? curval : newval;

    if (LOGDEBUG) {
        log_debug("\nEnter simple_list_test_simple_list_theList_theVal_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }
    return res;

} /* simple_list_test_simple_list_theList_theVal_edit */


/********************************************************************
* FUNCTION simple_list_test_simple_list_theList_edit
* 
* Edit database object callback
* Path: /simple_list/theList
* Add object instrumentation in COMMIT phase.
* 
* INPUTS:
*     see agt/agt_cb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t simple_list_test_simple_list_theList_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    val_value_t *errorval = (curval) ? curval : newval;

    if (LOGDEBUG) {
        log_debug("\nEnter simple_list_test_simple_list_theList_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }
    return res;

} /* simple_list_test_simple_list_theList_edit */


/********************************************************************
* FUNCTION simple_list_test_simple_list_edit
* 
* Edit database object callback
* Path: /simple_list
* Add object instrumentation in COMMIT phase.
* 
* INPUTS:
*     see agt/agt_cb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t simple_list_test_simple_list_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    val_value_t *errorval = (curval) ? curval : newval;

    if (LOGDEBUG) {
        log_debug("\nEnter simple_list_test_simple_list_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }

        if (res == NO_ERR) {
            res = agt_check_cache(&simple_list_val, newval, curval, editop);
        }
        
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }
    return res;

} /* simple_list_test_simple_list_edit */


/********************************************************************
* FUNCTION y_simple_list_test_init
* 
* initialize the simple_list_test server instrumentation library
* 
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
* 
* RETURNS:
*     error status
********************************************************************/
status_t y_simple_list_test_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;
    agt_profile_t *agt_profile;

    y_simple_list_test_init_static_vars();

    /* change if custom handling done */
    if (xml_strcmp(modname, y_simple_list_test_M_simple_list_test)) {
        return ERR_NCX_UNKNOWN_MODULE;
    }

    if (revision && xml_strcmp(revision, y_simple_list_test_R_simple_list_test)) {
        return ERR_NCX_WRONG_VERSION;
    }

    agt_profile = agt_get_profile();

    res = ncxmod_load_module(
        y_simple_list_test_M_simple_list_test,
        y_simple_list_test_R_simple_list_test,
        &agt_profile->agt_savedevQ,
        &simple_list_test_mod);
    if (res != NO_ERR) {
        return res;
    }

    simple_list_obj = ncx_find_object(
        simple_list_test_mod,
        y_simple_list_test_N_simple_list);
    if (simple_list_test_mod == NULL) {
        return SET_ERROR(ERR_NCX_DEF_NOT_FOUND);
    }
    
    res = agt_cb_register_callback(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list",
        (const xmlChar *)"2008-11-20",
        simple_list_test_simple_list_edit);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_callback(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList",
        (const xmlChar *)"2008-11-20",
        simple_list_test_simple_list_theList_edit);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_callback(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList/theKey",
        (const xmlChar *)"2008-11-20",
        simple_list_test_simple_list_theList_theKey_edit);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_callback(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList/theVal",
        (const xmlChar *)"2008-11-20",
        simple_list_test_simple_list_theList_theVal_edit);
    if (res != NO_ERR) {
        return res;
    }

    /* put your module initialization code here */
    
    return res;
} /* y_simple_list_test_init */


/********************************************************************
* FUNCTION y_simple_list_test_init2
* 
* SIL init phase 2: non-config data structures
* Called after running config is loaded
* 
* RETURNS:
*     error status
********************************************************************/
status_t y_simple_list_test_init2 (void)
{
    status_t res = NO_ERR;

    simple_list_val = agt_init_cache(
        y_simple_list_test_M_simple_list_test,
        y_simple_list_test_N_simple_list,
        &res);
    if (res != NO_ERR) {
        return res;
    }

    /* put your init2 code here */

    return res;
} /* y_simple_list_test_init2 */


/********************************************************************
* FUNCTION y_simple_list_test_cleanup
*    cleanup the server instrumentation library
* 
********************************************************************/
void y_simple_list_test_cleanup (void)
{
    agt_cb_unregister_callbacks(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list");

    agt_cb_unregister_callbacks(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList");

    agt_cb_unregister_callbacks(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList/theKey");

    agt_cb_unregister_callbacks(
        y_simple_list_test_M_simple_list_test,
        (const xmlChar *)"/simple_list/theList/theVal");

    /* put your cleanup code here */
    
} /* y_simple_list_test_cleanup */

/* END simple_list_test.c */