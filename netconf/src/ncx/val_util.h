#ifndef _H_val_util
#define _H_val_util

/*  FILE: val_util.h
*********************************************************************
*								    *
*			 P U R P O S E				    *
*								    *
*********************************************************************

    Value Struct Utilities

*********************************************************************
*								    *
*		   C H A N G E	 H I S T O R Y			    *
*								    *
*********************************************************************

date	     init     comment
----------------------------------------------------------------------
19-dec-05    abb      Begun
21jul08      abb      start obj-based rewrite
29jul08      abb      split out from val.h

*/

#include <xmlstring.h>

#ifndef _H_ncxtypes
#include "ncxtypes.h"
#endif

#ifndef _H_obj
#include "obj.h"
#endif

#ifndef _H_op
#include "op.h"
#endif

#ifndef _H_status
#include "status.h"
#endif

#ifndef _H_val
#include "val.h"
#endif

#ifndef _H_xmlns
#include "xmlns.h"
#endif

#ifndef _H_xpath
#include "xpath.h"
#endif


/********************************************************************
*                                                                   *
*                            T Y P E S                              *
*                                                                   *
*********************************************************************/


/* user function callback template to test output 
 * of a specified node.
 *
 * val_nodetest_fn_t
 * 
 *  Run a user-defined test on the supplied node, and 
 *  determine if it should be output or not.
 *
 * INPUTS:
 *   withdef == with-defaults value in affect
 *   nodetyp == node type enumeration
 *   mode == void pointer to the node to check
 *
 * RETURNS:
 *   TRUE if the node should be output
 *   FALSE if the node should be skipped
 */
typedef boolean 
    (*val_nodetest_fn_t) (ncx_withdefaults_t withdef,
			  const val_value_t *node);


/********************************************************************
*								    *
*			F U N C T I O N S			    *
*								    *
*********************************************************************/

extern void
    val_set_canonical_order (val_value_t *val);

extern status_t 
    val_gen_index_comp  (const obj_key_t *in,
			 val_value_t *val);

extern status_t 
    val_gen_key_entry  (val_value_t *keyval);

extern status_t 
    val_gen_index_chain (const obj_template_t *obj,
			 val_value_t *val);

/* add defaults to an initialized complex value */
extern status_t 
    val_add_defaults (val_value_t *val,
		      boolean scriptmode);

extern status_t
    val_instance_check (val_value_t  *val,
			const obj_template_t *obj);

extern val_value_t *
    val_get_choice_first_set (val_value_t *val,
			      const obj_template_t *obj);

extern val_value_t *
    val_get_choice_next_set (val_value_t *val,
			     const obj_template_t *obj,
			     val_value_t *curchild);

extern boolean
    val_choice_is_set (val_value_t *val,
		       const obj_template_t *obj);

extern void
    val_purge_errors_from_root (val_value_t *val);

extern val_value_t *
    val_new_child_val (xmlns_id_t   nsid,
		       const xmlChar *name,
		       boolean copyname,
		       val_value_t *parent,
		       op_editop_t editop);


extern status_t
    val_gen_instance_id (xml_msg_hdr_t *mhdr,
			 const val_value_t  *val, 
			 ncx_instfmt_t format,
			 xmlChar  **buff);


extern status_t
    val_gen_split_instance_id (xml_msg_hdr_t *mhdr,
			       const val_value_t  *val, 
			       ncx_instfmt_t format,
			       xmlns_id_t leaf_nsid,
			       const xmlChar *leaf_name,
			       xmlChar  **buff);

extern status_t
    val_get_index_string (xml_msg_hdr_t *mhdr,
			  ncx_instfmt_t format,
			  const val_value_t *val,
			  xmlChar *buff,
			  uint32  *len);

extern status_t
    val_check_child_conditional (val_value_t *val,
				 val_value_t *valroot,
				 const obj_template_t *childobj,
				 boolean *condresult);

extern boolean
    val_is_mandatory (val_value_t *val,
		      val_value_t *valroot,
		      const obj_template_t *childobj);


extern ncx_iqual_t 
    val_get_cond_iqualval (val_value_t *val,
			   val_value_t *valroot,
			   const obj_template_t *obj);

extern xpath_pcb_t *
    val_get_xpathpcb (val_value_t *val);

extern const xpath_pcb_t *
    val_get_const_xpathpcb (const val_value_t *val);

extern val_value_t *
    val_make_simval_obj (const obj_template_t *obj,
			 const xmlChar *valstr,
			 status_t  *res);

extern status_t 
    val_set_simval_obj (val_value_t  *val,
			const obj_template_t *obj,
			const xmlChar *valstr);


#endif	    /* _H_val_util */
