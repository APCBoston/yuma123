/*  FILE: xml_util.c

    General Utilities that simplify usage of the libxml functions


*********************************************************************
*                                                                   *
*                  C H A N G E   H I S T O R Y                      *
*                                                                   *
*********************************************************************

date         init     comment
----------------------------------------------------------------------
13oct05      abb      begun

*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include <xmlstring.h>
#include <xmlreader.h>

#ifndef _H_procdefs
#include "procdefs.h"
#endif

#ifndef _H_def_reg
#include "def_reg.h"
#endif

#ifndef _H_log
#include "log.h"
#endif

#ifndef _H_ncx
#include "ncx.h"
#endif

#ifndef _H_rpc
#include "rpc.h"
#endif

#ifndef _H_status
#include "status.h"
#endif

#ifndef _H_xmlns
#include "xmlns.h"
#endif

#ifndef _H_xml_util
#include "xml_util.h"
#endif


/********************************************************************
*                                                                   *
*                          C O N S T A N T S                        *
*                                                                   *
*********************************************************************/

#ifdef DEBUG
#define XML_UTIL_DEBUG  1
#endif

#define XML_READER_OPTIONS    XML_PARSE_RECOVER+XML_PARSE_NOERROR+\
	XML_PARSE_NOWARNING+XML_PARSE_NOBLANKS+XML_PARSE_NONET

#define XML_SES_URL "netconf://pdu"

/********************************************************************
*								    *
*			     T Y P E S				    *
*								    *
*********************************************************************/

/* One entry to find a character entity value */
typedef struct xml_chcvt_t_ {
    const xmlChar   *chstr;
    xmlChar          ch;
} xml_chcvt_t;


/********************************************************************
*                                                                   *
*                       V A R I A B L E S			    *
*                                                                   *
*********************************************************************/

/* lookup list for the char entities that need to be converted */
static xml_chcvt_t chcvt [] = {
    {(const xmlChar *)"gt", '>'},
    {(const xmlChar *)"lt", '<'},
    {(const xmlChar *)"quot", '"'},
    {(const xmlChar *)"amp", '&'},
    {(const xmlChar *)"lt", '<'},
    {NULL, ' '}
};

/************** E X T E R N A L   F U N C T I O N S   **************/


/********************************************************************
* FUNCTION xml_new_node
* 
* Malloc and init a new xml_node_t struct
*
* RETURNS:
*   pointer to new node or NULL if malloc error
*********************************************************************/
xml_node_t *
    xml_new_node (void)
{
    xml_node_t *node;

    node = m__getObj(xml_node_t);
    if (!node) {
	return NULL;
    }
    xml_init_node(node);
    return node;

} /* xml_new_node */


/********************************************************************
* FUNCTION xml_init_node
* 
* Init an xml_node_t struct
*
* INPUTS:
*   node == pointer to node to init
*********************************************************************/
void
    xml_init_node (xml_node_t *node)
{
#ifdef DEBUG   
    if (!node) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    memset(node, 0x0, sizeof(xml_node_t));
    xml_init_attrs(&node->attrs);
} /* xml_init_node */


/********************************************************************
* FUNCTION xml_free_node
* 
* Free an xml_node_t struct
*
* INPUTS:
*   node == pointer to node to free
*********************************************************************/
void
    xml_free_node (xml_node_t *node)
{
#ifdef DEBUG
    if (!node) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif
    xml_clean_node(node);
    m__free(node);
} /* xml_free_node */


/********************************************************************
* FUNCTION xml_clean_node
* 
* Clean an xml_node_t struct
*
* INPUTS:
*   node == pointer to node to clean
*********************************************************************/
void
    xml_clean_node (xml_node_t *node)
{
#ifdef DEBUG
    if (!node) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    node->nodetyp = XML_NT_NONE;
    node->nsid = 0;
    node->module = NULL;
    node->qname = NULL;
    node->elname = NULL;
    if (node->simfree) {
	m__free(node->simfree);
	node->simfree = NULL;
    }
    node->simval = NULL;
    node->simlen = 0;
    node->depth = 0;
    xml_clean_attrs(&node->attrs);

} /* xml_clean_node */


/********************************************************************
* FUNCTION xml_get_reader_from_filespec
* 
* Get a new xmlTextReader for parsing a debug test file
*
* INPUTS:
*   filespec == full filename including path of the 
*               XML instance document to parse
* OUTPUTS:
*   *reader == pointer to new reader or NULL if some error
*
* RETURNS:
*   status of the operation
*********************************************************************/
status_t
    xml_get_reader_from_filespec (const char *filespec,
				  xmlTextReaderPtr  *reader)
{
    int        options;

#ifdef DEBUG
    if (!filespec || !reader) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    } 
#endif

    /* the parser option settings are critical!!!
     * The parser routines assume that includes will be 
     * expanded in place and #TEXT nodes such as EOLN will
     * be omitted by the parser
     */
    options = XML_READER_OPTIONS + XML_PARSE_XINCLUDE;
    *reader = xmlReaderForFile(filespec, NULL, options);
    if (*reader==NULL) {
	return ERR_XML_READER_START_FAILED;
    }
    return NO_ERR;

} /* xml_get_reader_from_filespec */


/********************************************************************
* FUNCTION xml_get_reader_for_session
* 
* Get a new xmlTextReader for parsing the input of a NETCONF session
*
* INPUTS:
*   readfn == IO read function to use for this xmlTextReader
*   closefn == IO close fn to use for this xmlTextReader
*   context == the ses_cb_t pointer passes as a void *
*              this will be passed to the read and close functions
* OUTPUTS:
*   *reader == pointer to new reader or NULL if some error
* RETURNS:
*   status of the operation
*********************************************************************/
status_t
    xml_get_reader_for_session (xmlInputReadCallback readfn,
				xmlInputCloseCallback closefn,
				void *context,
				xmlTextReaderPtr  *reader)
{
    int        options;

#ifdef DEBUG
    if (!readfn || !reader) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    } 
#endif

    /* the parser option settings are critical!!!
     * The parser routines assume that includes will be 
     * expanded in place and #TEXT nodes such as EOLN will
     * be omitted by the parser
     */
    options = XML_READER_OPTIONS;
    *reader = xmlReaderForIO(readfn, closefn, context, XML_SES_URL, 
			     NULL, options);
    if (*reader==NULL) {
	return ERR_XML_READER_START_FAILED;
    }
    return NO_ERR;

} /* xml_get_reader_for_session */


/********************************************************************
* FUNCTION xml_reset_reader_for_session
* 
* Reset the xmlTextReader for parsing the input of a NETCONF session
*
* INPUTS:
*   readfn == IO read function to use for this xmlTextReader
*   closefn == IO close fn to use for this xmlTextReader
*   context == the ses_cb_t pointer passes as a void *
*              this will be passed to the read and close functions
* OUTPUTS:
*   *reader == pointer to new reader or NULL if some error
* RETURNS:
*   status of the operation
*********************************************************************/
status_t
    xml_reset_reader_for_session (xmlInputReadCallback readfn,
				  xmlInputCloseCallback closefn,
				  void *context,
				  xmlTextReaderPtr  reader)
{
    int  ret, options;

#ifdef DEBUG
    if (!readfn || !reader) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    } 
#endif

    /* the parser option settings are critical!!!
     * The parser routines assume that includes will be 
     * expanded in place and #TEXT nodes such as EOLN will
     * be omitted by the parser
     */
    options = XML_READER_OPTIONS;
    ret = xmlReaderNewIO(reader, readfn, closefn, context,
			 XML_SES_URL, NULL, XML_READER_OPTIONS);
    if (ret != 0) {
	return ERR_XML_READER_START_FAILED;
    }
    return NO_ERR;

} /* xml_reset_reader_for_session */


/********************************************************************
* FUNCTION xml_free_reader
* 
* Free the previously allocated xmlTextReader
*
* INPUTS:
*   reader == xmlTextReader to close and deallocate
* RETURNS:
*   none
*********************************************************************/
void
    xml_free_reader (xmlTextReaderPtr reader)
{
#ifdef DEBUG
    if (!reader) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif
    xmlFreeTextReader(reader);

} /* xml_free_reader */


/********************************************************************
* FUNCTION xml_get_node_name
* 
* get the node type according to the xmlElementType enum list
* in /usr/include/libxml/libxml/tree.h
*
* INPUTS:
*   nodeval == integer node type from system
* RETURNS:
*   string corresponding to the integer value
*********************************************************************/
const char * 
    xml_get_node_name (int nodeval)
{
    switch (nodeval) {
    case 1: return "XML_ELEMENT_NODE";
    case 2: return "XML_ATTRIBUTE_NODE";
    case 3: return "XML_TEXT_NODE";
    case 4: return "XML_CDATA_SECTION_NODE";
    case 5: return "XML_ENTITY_REF_NODE";
    case 6: return "XML_ENTITY_NODE";
    case 7: return "XML_PI_NODE";
    case 8: return "XML_COMMENT_NODE";
    case 9: return "XML_DOCUMENT_NODE";
    case 10: return "XML_DOCUMENT_TYPE_NODE";
    case 11: return "XML_DOCUMENT_FRAG_NODE";
    case 12: return "XML_NOTATION_NODE";
    case 13: return "XML_HTML_DOCUMENT_NODE";
    case 14: return "XML_DTD_NODE";
    case 15: return "XML_ELEMENT_DECL";
    case 16: return "XML_ATTRIBUTE_DECL";
    case 17: return "XML_ENTITY_DECL";
    case 18: return "XML_NAMESPACE_DECL";
    case 19: return "XML_XINCLUDE_START";
    case 20: return "XML_XINCLUDE_END";
    case 21: return "XML_DOCB_DOCUMENT_NODE";
    default: return "**unknown**";
    }
    /*NOTREACHED*/
}  /* xml_get_node_name */


/********************************************************************
* FUNCTION xml_advance_reader
* 
* Advance to the next node in the specified reader
*
* INPUTS:
*   reader == XmlReader already initialized from File, Memory,
*             or whatever
* RETURNS:
*   FALSE if OEF seen, or TRUE if normal
*********************************************************************/
boolean 
    xml_advance_reader (xmlTextReaderPtr reader)
{
    int  ret;

#ifdef DEBUG
    if (!reader) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    /* advance the node pointer */
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
	return FALSE;
    }
    return TRUE;

}  /* xml_advance_reader */


/********************************************************************
* FUNCTION xml_node_match
*
* check if a specific node is the proper owner, name, and type
*
* INPUTS:
*    node == node to match against
*    nsid == namespace ID to match (0 == match any)
*    elname == element name to match (NULL == match any)
*    nodetyp == node type to match (XML_NT_NONE == match any)
* RETURNS:
*    status
*********************************************************************/
status_t 
    xml_node_match (const xml_node_t *node,
		    xmlns_id_t        nsid,
		    const xmlChar *elname,
		    xml_nodetyp_t  nodetyp)
{
#ifdef DEBUG
    if (!node) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (nsid) {
	/* namespace error only if namespaces are being used */
	if (node->nsid && node->nsid != nsid) {
	    return ERR_NCX_WRONG_NAMESPACE;
	}
    }

    if (elname) {
	if (!node->elname) {
	    return ERR_NCX_UNKNOWN_ELEMENT;
	}
	if (xml_strcmp(elname, node->elname)) {
	    return ERR_NCX_WRONG_ELEMENT;
	}
    }

    if (nodetyp != XML_NT_NONE) {
	if (nodetyp != node->nodetyp) {
	    return ERR_NCX_WRONG_NODETYP;
	}
    }

    return NO_ERR;

} /* xml_node_match */


/********************************************************************
* FUNCTION xml_endnode_match
*
* check if a specific node is the proper endnode match
* for a given startnode
*
* INPUTS:
*    startnode == start node to match against
*    endnode == potential end node to test
* RETURNS:
*    status,
*********************************************************************/
status_t 
    xml_endnode_match (const xml_node_t *startnode,
		       const xml_node_t *endnode)
{
#ifdef DEBUG
    if (!startnode || !endnode) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (endnode->nodetyp != XML_NT_END) {
	return ERR_NCX_WRONG_NODETYP;
    }

    if (startnode->depth != endnode->depth) {
	return ERR_NCX_WRONG_NODEDEPTH;
    }

    if (xml_strcmp(startnode->elname, endnode->elname)) {
	return ERR_NCX_UNKNOWN_ELEMENT;
    }

    if (startnode->nsid && !endnode->nsid) {
	return ERR_NCX_UNKNOWN_NAMESPACE;
    }

    if (startnode->nsid != endnode->nsid) {
	return ERR_NCX_WRONG_NAMESPACE;
    }

    return NO_ERR;

} /* xml_endnode_match */


/********************************************************************
* FUNCTION xml_docdone
*
* check if the input is completed for a given PDU
*
* INPUTS:
*    reader == xml text reader
* RETURNS:
*    TRUE if document is done
*    FALSE if document is not done or some error
*    If a node is read, the reader will be pointing to that node
*********************************************************************/
boolean 
    xml_docdone (xmlTextReaderPtr reader)
{
    int  ret;

#ifdef DEBUG
    if (!reader) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    /* advance the node pointer */
    ret = xmlTextReaderRead(reader);
    if (ret != 1) {
	return TRUE;   /* assume ERR_XML_READER_EOF !! */
    } else {
	return FALSE;  /* read was successful, doc is not done */
    }
} /* xml_docdone */


/********************************************************************
* FUNCTION xml_dump_node
*
* Debug function to printf xml_node_t contents
*
* INPUTS:
*    node == node to dump
*********************************************************************/
void
    xml_dump_node (const xml_node_t *node)
{

    const char *typ;
    boolean  nam, ok;
    const xml_attr_t  *attr;

#ifdef DEBUG
    if (!node) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    switch (node->nodetyp) {
    case XML_NT_NONE:
	typ = "NONE";
	ok = nam = FALSE;
	break;
    case XML_NT_EMPTY:
	typ = "EMPTY";
	ok = nam = TRUE;
	break;
    case XML_NT_START:
	typ = "START";
	ok = nam = TRUE;
	break;
    case XML_NT_END:
	typ = "END";
	ok = nam = TRUE;
	break;
    case XML_NT_STRING:
	typ = "STRING";
	nam = FALSE;
	ok = TRUE;
	break;
    default:
	typ = "ERR";
	ok = nam = FALSE;
	break;
    }

    if (ok) {
	log_write("\nXML node (%d:%d): %s %s",
		  node->nsid, node->depth, typ, 
		  (nam) ? (const char *)node->elname : "");

	if (node->simval) {
	    log_write("\n   val(%u):%s", node->simlen, node->simval);
	}
	for (attr = (const xml_attr_t *)dlq_firstEntry(&node->attrs);
	     attr != NULL;
	     attr = (const xml_attr_t *)dlq_nextEntry(attr)) {
	    log_write("\n   attr: ns:%d name:%s (%s)",
		   attr->attr_ns, (const char *)attr->attr_name, 
		   (const char *)attr->attr_val);
	}
    } else {
	log_write("\nXML node ERR (%s)", typ);
    }
    log_write("\n");

} /* xml_dump_node */


/********************************************************************
* FUNCTION xml_init_attrs
*
* initialize an xml_attrs_t variable
*
* INPUTS:
*    attrs == attribute queue to init
* RETURNS:
*    none
*********************************************************************/
void
    xml_init_attrs (xml_attrs_t *attrs)
{
#ifdef DEBUG
    if (!attrs) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    dlq_createSQue(attrs);

}  /* xml_init_attrs */


/********************************************************************
* FUNCTION xml_new_attr
*
* malloc and init an attribute struct
*
* INPUTS:
*    none
* RETURNS:
*    pointer to new xml_attr_t or NULL if malloc error
*********************************************************************/
xml_attr_t *
    xml_new_attr (void)
{
    xml_attr_t  *attr;

    attr = m__getObj(xml_attr_t);
    if (!attr) {
        return NULL;
    }
    memset(attr, 0x0, sizeof(xml_attr_t));
    return attr;

}  /* xml_new_attr */


/********************************************************************
* FUNCTION xml_add_attr
*
* add an attribute to an attribute list
*
* INPUTS:
*    none
* RETURNS:
*    pointer to new xml_attr_t or NULL if malloc error
*********************************************************************/
void
    xml_free_attr (xml_attr_t *attr)
{
#ifdef DEBUG
    if (!attr) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif
    if (attr->attr_dname) {
	m__free(attr->attr_dname);
    }
    if (attr->attr_val) {
	m__free(attr->attr_val);
    }
    m__free(attr);

}  /* xml_free_attr */


/********************************************************************
* FUNCTION xml_add_attr
*
* add an attribute to an attribute list
*
* INPUTS:
*    attrs == attribute queue to init
*    ns_id == namespace ID (use XMLNS_NONE if no prefix desired)
*    attr_name == attribute name string
*    attr_val == attribute value string
* RETURNS:
*    NO_ERR if all okay
*********************************************************************/
status_t
    xml_add_attr (xml_attrs_t *attrs, 
		  xmlns_id_t  ns_id,
		  const xmlChar *attr_name,
		  const xmlChar *attr_val)
{
    xml_attr_t  *attr;

#ifdef DEBUG
    if (!attrs || !attr_name || !attr_val) {
        return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    attr = xml_new_attr();
    if (!attr) {
        return ERR_INTERNAL_MEM;
    }
    attr->attr_dname = xml_strdup(attr_name);
    attr->attr_name = attr->attr_dname;
    attr->attr_qname = attr->attr_dname;
    if (!attr->attr_dname) {
	xml_free_attr(attr);
        return SET_ERROR(ERR_INTERNAL_MEM);
    }

    attr->attr_val = xml_strdup(attr_val);
    if (!attr->attr_val) {
	xml_free_attr(attr);
        return SET_ERROR(ERR_INTERNAL_MEM);
    }
    attr->attr_ns = ns_id;
    dlq_enque(attr, attrs);
    return NO_ERR;

}  /* xml_add_attr */


/********************************************************************
* FUNCTION xml_add_qattr
*
* add a qualified attribute to an attribute list with a prefix
*
* INPUTS:
*    attrs == attribute queue to init
*    ns_id == namespace ID (use XMLNS_NONE if no prefix desired)
*    attr_qname == qualified attribute name string
*    plen == attribute prefix length
*    attr_val == attribute value string
* RETURNS:
*    NO_ERR if all okay
*********************************************************************/
status_t
    xml_add_qattr (xml_attrs_t *attrs, 
		   xmlns_id_t  ns_id,
		   const xmlChar *attr_qname,
		   uint32  plen,
		   const xmlChar *attr_val)
{
    xml_attr_t  *attr;

#ifdef DEBUG
    if (!attrs || !attr_qname || !attr_val) {
        return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    attr = xml_new_attr();
    if (!attr) {
        return ERR_INTERNAL_MEM;
    }
    attr->attr_dname = xml_strdup(attr_qname);
    if (!attr->attr_dname) {
	xml_free_attr(attr);
        return ERR_INTERNAL_MEM;
    }
    attr->attr_qname = attr->attr_dname;
    attr->attr_name = attr->attr_dname+plen;

    attr->attr_val = xml_strdup(attr_val);
    if (!attr->attr_val) {
	xml_free_attr(attr);
        return SET_ERROR(ERR_INTERNAL_MEM);
    }
    attr->attr_ns = ns_id;
    dlq_enque(attr, attrs);
    return NO_ERR;

}  /* xml_add_qattr */


/********************************************************************
* FUNCTION xml_add_xmlns_attr
*
* add an xmlns decl to the attribute Queue
*
* INPUTS:
*    attrs == attribute queue to add to
*    ns_id == namespace ID of the xmlns target
*    pfix == namespace prefix string assigned
*         == NULL for default namespace
*
* RETURNS:
*    NO_ERR if all okay
*********************************************************************/
status_t
    xml_add_xmlns_attr (xml_attrs_t *attrs, 
			xmlns_id_t  ns_id,
			const xmlChar *pfix)
{
    xml_attr_t    *attr;
    xmlChar       *s;
    const xmlChar *nsval;
    uint32         len;

#ifdef DEBUG
    if (!attrs) {
        return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    /* get a new attr struct */
    attr = xml_new_attr();
    if (!attr) {
        return ERR_INTERNAL_MEM;
    }

    /* get the namespace URI value */
    nsval = xmlns_get_ns_name(ns_id);
    if (!nsval) {
	return SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* copy the namespace URI as the attr value */
    attr->attr_val = xml_strdup(nsval);
    if (!attr->attr_val) {
	xml_free_attr(attr);
        return SET_ERROR(ERR_INTERNAL_MEM);
    }

    /* get the dname buffer length to malloc */
    len = XMLNS_LEN+1;
    if (pfix) {
	len += (xml_strlen(pfix) + 1);
    }

    /* get a name buffer */
    attr->attr_dname = m__getMem(len);
    if (!attr->attr_dname) {
	xml_free_attr(attr);
        return ERR_INTERNAL_MEM;
    }
    attr->attr_qname = attr->attr_dname;

    /* construct an xmlns:prefix string in the name buffer */
    s = attr->attr_dname;    
    s += xml_strcpy(attr->attr_dname, XMLNS);

    /* point the name field at the prefix value if there is one */
    if (pfix) {
	*s++ = XMLNS_SEPCH;
	attr->attr_name = s;
	while (*pfix) {
	    *s++ = *pfix++;
	}
    } else {
	attr->attr_name = attr->attr_dname;
    }
    *s = 0;

    attr->attr_ns = xmlns_ns_id();
    attr->attr_xmlns_ns = ns_id;
    dlq_enque(attr, attrs);
    return NO_ERR;

}  /* xml_add_xmlns_attr */


/********************************************************************
* FUNCTION xml_add_inv_xmlns_attr
*
* add an xmlns decl to the attribute Queue
* for an INVALID namespace.  This is needed for the
* error-info element within the rpc-error report
*
* INPUTS:
*    attrs == attribute queue to add to
*    ns_id == namespace ID of the xmlns target
*    pfix == namespace prefix string assigned
*         == NULL for default namespace
*    nsval == namespace URI value of invalid namespace
*
* RETURNS:
*    NO_ERR if all okay
*********************************************************************/
status_t
    xml_add_inv_xmlns_attr (xml_attrs_t *attrs, 
			    xmlns_id_t  ns_id,
			    const xmlChar *pfix,
			    const xmlChar *nsval)
{
    xml_attr_t    *attr;
    xmlChar       *s;
    uint32         len;

#ifdef DEBUG
    if (!attrs) {
        return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    /* get a new attr struct */
    attr = xml_new_attr();
    if (!attr) {
        return ERR_INTERNAL_MEM;
    }
    
    if (!nsval) {
	nsval = (const xmlChar *)"INVALID";
    }

    /* copy the namespace URI as the attr value */
    attr->attr_val = xml_strdup(nsval);
    if (!attr->attr_val) {
	xml_free_attr(attr);
        return SET_ERROR(ERR_INTERNAL_MEM);
    }

    /* get the dname buffer length to malloc */
    len = XMLNS_LEN+1;
    if (pfix) {
	len += (xml_strlen(pfix) + 1);
    }

    /* get a name buffer */
    attr->attr_dname = m__getMem(len);
    if (!attr->attr_dname) {
	xml_free_attr(attr);
        return ERR_INTERNAL_MEM;
    }
    attr->attr_qname = attr->attr_dname;

    /* construct an xmlns:prefix string in the name buffer */
    s = attr->attr_dname;    
    s += xml_strcpy(attr->attr_dname, XMLNS);

    /* point the name field at the prefix value if there is one */
    if (pfix) {
	*s++ = XMLNS_SEPCH;
	attr->attr_name = s;
	while (*pfix) {
	    *s++ = *pfix++;
	}
    } else {
	attr->attr_name = attr->attr_dname;
    }
    *s = 0;

    attr->attr_ns = xmlns_ns_id();
    attr->attr_xmlns_ns = ns_id;
    dlq_enque(attr, attrs);
    return NO_ERR;

}  /* xml_add_inv_xmlns_attr */


/********************************************************************
* FUNCTION xml_first_attr
*
* get the first attribute in the list
*
* INPUTS:
*    attrs == attribute queue to get from
* RETURNS:
*    pointer to first entry or NULL if none
*********************************************************************/
xml_attr_t * 
    xml_first_attr (xml_attrs_t  *attrs)
{
#ifdef DEBUG
    if (!attrs) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return (xml_attr_t *)dlq_firstEntry(attrs);

}  /* xml_first_attr */


/********************************************************************
* FUNCTION xml_get_first_attr
*
* get the first attribute in the attrs list, from an xml_node_t param
*
* INPUTS:
*    node == node with the attrQ to use
* RETURNS:
*    pointer to first entry or NULL if none
*********************************************************************/
xml_attr_t * 
    xml_get_first_attr (const xml_node_t *node)
{
#ifdef DEBUG
    if (!node) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return (xml_attr_t *)dlq_firstEntry(&node->attrs);

}  /* xml_get_first_attr */


/********************************************************************
* FUNCTION xml_next_attr
*
* get the next attribute in the list
*
* INPUTS:
*    attr == attribute queue to init
* RETURNS:
*    pointer to the next entry or NULL if none
*********************************************************************/
xml_attr_t * 
    xml_next_attr (xml_attr_t  *attr)
{
#ifdef DEBUG
    if (!attr) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }	
#endif

    return (xml_attr_t *)dlq_nextEntry(attr);

}  /* xml_next_attr */


/********************************************************************
* FUNCTION xml_clean_attrs
*
* clean an xml_attrs_t variable
*
* INPUTS:
*    attrs == attribute queue to clean
* RETURNS:
*    none
*********************************************************************/
void 
    xml_clean_attrs (xml_attrs_t  *attrs)
{
    xml_attr_t  *attr;

#ifdef DEBUG
    if (!attrs) {
        SET_ERROR(ERR_INTERNAL_PTR);
        return;
    }
#endif

    while (!dlq_empty(attrs)) {
	attr = (xml_attr_t *)dlq_deque(attrs);
	xml_free_attr(attr);
    }

}  /* xml_clean_attrs */


/********************************************************************
* FUNCTION xml_find_attr
* 
*  Must be called after xxx_xml_consume_node
*  Go looking for the specified attribute node
* INPUTS:
*   node == xml_node_t to check
*   nsid == namespace ID of attribute to find
*   attrname == attribute name to find
* RETURNS:
*   pointer to found xml_attr_t or NULL if not found
*********************************************************************/
xml_attr_t *
    xml_find_attr (xml_node_t *node,
		   xmlns_id_t        nsid,
		   const xmlChar    *attrname)
{
#ifdef DEBUG
    if (!node || !attrname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return xml_find_attr_q(&node->attrs, nsid, attrname);

}  /* xml_find_attr */


/********************************************************************
* FUNCTION xml_find_attr_q
* 
*  Must be called after xxx_xml_consume_node
*  Go looking for the specified attribute node
* INPUTS:
*   node == xml_node_t to check
*   nsid == namespace ID of attribute to find
*   attrname == attribute name to find
* RETURNS:
*   pointer to found xml_attr_t or NULL if not found
*********************************************************************/
xml_attr_t *
    xml_find_attr_q (xml_attrs_t *attrs,
		     xmlns_id_t        nsid,
		     const xmlChar    *attrname)
{
    xml_attr_t *attr;

#ifdef DEBUG
    if (!attrs || !attrname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    for (attr = (xml_attr_t *)dlq_firstEntry(attrs);
	 attr != NULL;
	 attr = (xml_attr_t *)dlq_nextEntry(attr)) {
	if (nsid && attr->attr_ns) {
	    if (nsid==attr->attr_ns && 
		!xml_strcmp(attr->attr_name, attrname)) {
		return attr;
	    }
	} else if (!xml_strcmp(attr->attr_name, attrname)) {
	    return attr;
	}
    }
    return NULL;

}  /* xml_find_attr_q */


/********************************************************************
* FUNCTION xml_find_ro_attr
* 
*  Must be called after xxx_xml_consume_node
*  Go looking for the specified attribute node
* INPUTS:
*   node == xml_node_t to check
*   nsid == namespace ID of attribute to find
*   attrname == attribute name to find
* RETURNS:
*   pointer to found xml_attr_t or NULL if not found
*********************************************************************/
const xml_attr_t *
    xml_find_ro_attr (const xml_node_t *node,
		   xmlns_id_t        nsid,
		   const xmlChar    *attrname)
{
    const xml_attr_t *attr;

#ifdef DEBUG
    if (!node || !attrname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    for (attr = (const xml_attr_t *)dlq_firstEntry(&node->attrs);
	 attr != NULL;
	 attr = (const xml_attr_t *)dlq_nextEntry(attr)) {
	if (nsid && attr->attr_ns) {
	    if (nsid==attr->attr_ns && 
		!xml_strcmp(attr->attr_name, attrname)) {
		return attr;
	    }
	} else if (!xml_strcmp(attr->attr_name, attrname)) {
	    return attr;
	}
    }
    return NULL;

}  /* xml_find_ro_attr */


/********************************************************************
* FUNCTION xml_strlen
* 
* String len for xmlChar -- does not check for buffer overflow
* INPUTS:
*   str == buffer to check
* RETURNS:
*   number of xmlChars before null terminator found
*********************************************************************/
uint32 
    xml_strlen (const xmlChar *str)
{
    uint32  len = 0;

#ifdef DEBUG
    if (!str) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    while (*str++) {
	len++;
    }
    return len;

}  /* xml_strlen */


/********************************************************************
* FUNCTION xml_strlen_sp
* 
* String len for xmlChar -- does not check for buffer overflow
* Check for any whitespace in the string as well
*
* INPUTS:
*   str == buffer to check
*   sp == address of any-spaces-test output
*
* OUTPUTS:
*   *sp == TRUE if any whitespace found in the string
*
* RETURNS:
*   number of xmlChars before null terminator found
*********************************************************************/
uint32 
    xml_strlen_sp (const xmlChar *str,
		   boolean *sp)
{
    uint32   len = 0;

#ifdef DEBUG
    if (!str || !sp) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    *sp = FALSE;
    for (;;) {
	if (!*str) {
	    return len;
	} else if (!*sp) {
	    if (xml_isspace(*str)) {
		*sp = TRUE;
	    }
	}
	str++;
	len++;
    }
    /*NOTREACHED*/

}  /* xml_strlen_sp */


/********************************************************************
* FUNCTION xml_strcpy
* 
* String copy for xmlChar -- does not check for buffer overflow
*
* Even if return value is zero, the EOS char is copied
*
* INPUTS:
*   copyTo == buffer to copy into
*   copyFrom == zero-terminated xmlChar string to copy from
* RETURNS:
*   number of bytes copied to the result buffer, not including EOS
*********************************************************************/
uint32 
    xml_strcpy (xmlChar *copyTo, 
		const xmlChar *copyFrom)
{
    uint32  cnt;

#ifdef DEBUG
    if (!copyTo || !copyFrom) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif
    
    cnt = 0;
    while ((*copyTo++ = *copyFrom++) != (xmlChar)0x0) {
	cnt++;
    }
    return cnt;
    
}  /* xml_strcpy */


/********************************************************************
* FUNCTION xml_strncpy
* 
* String copy for xmlChar -- checks for buffer overflow
* INPUTS:
*   copyTo == buffer to copy into
*   copyFrom == zero-terminated xmlChar string to copy from
*   maxLen == max number of xmlChars to copy, but does include 
*             the terminating zero that is added if this max is reached
* RETURNS:
*   number of bytes copied to the result buffer
*********************************************************************/
uint32
    xml_strncpy (xmlChar *copyTo, 
		 const xmlChar *copyFrom,
		 uint32  maxlen)
{
    uint32  i;

#ifdef DEBUG
    if (!copyTo || !copyFrom) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif
    
    for (i=0; i<maxlen; i++) {
	if ((*copyTo++ = *copyFrom++) == (xmlChar)0x0) {
	    return i;
	}
    }

    /* if we get here then terminate the partial string */
    *copyTo = 0;
    return maxlen;

}   /* xml_strncpy */


/********************************************************************
* FUNCTION xml_strdup
* 
* String duplicate for xmlChar
* INPUTS:
*   copyFrom == zero-terminated xmlChar string to copy from
* RETURNS:
*   pointer to new string or NULL if some error
*********************************************************************/
xmlChar * 
    xml_strdup (const xmlChar *copyFrom)
{
    
    xmlChar *str;
    uint32  len, i;

#ifdef DEBUG
    if (!copyFrom) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    len = xml_strlen(copyFrom);

    /* get a string  buffer */
    str = (xmlChar *)m__getMem(len+1);
    if (str==NULL) {
	return NULL;
    }

    /* copy the string */
    for (i=0; i<len; i++) {
	str[i] = *copyFrom++;
    }
    str[len] = 0;
    return str;

}  /* xml_strdup */


/********************************************************************
* FUNCTION xml_strcat
* 
* String concatenate for xmlChar
* INPUTS:
*   appendTo == zero-terminated xmlChar string to append to
*   appendFrom == zero-terminated xmlChar string to append from
* RETURNS:
*   appendTo if no error or NULL if some error
*********************************************************************/
xmlChar * 
    xml_strcat (xmlChar *appendTo,
		const xmlChar *appendFrom)
{
    uint32  len;

#ifdef DEBUG
    if (!appendTo || !appendFrom) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    len = xml_strlen(appendTo);
    while (*appendFrom) {
        appendTo[len++] = *appendFrom++;
    }
    appendTo[len] = 0;

    return appendTo;

}  /* xml_strcat */


/********************************************************************
* FUNCTION xml_strncat
* 
* String concatenate for at most maxlen xmlChars
* INPUTS:
*   appendTo == zero-terminated xmlChar string to append to
*   appendFrom == zero-terminated xmlChar string to append from
*   maxlen == max number of chars to append
* RETURNS:
*   appendTo if no error or NULL if some error
*********************************************************************/
xmlChar * 
    xml_strncat (xmlChar *appendTo,
		 const xmlChar *appendFrom,
		 uint32 maxlen)
{
    uint32  len;

#ifdef DEBUG
    if (!appendTo || !appendFrom || !maxlen) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    len = xml_strlen(appendTo);
    while (*appendFrom && maxlen--) {
        appendTo[len++] = *appendFrom++;
    }
    appendTo[len] = 0;

    return appendTo;

}  /* xml_strncat */


/********************************************************************
* FUNCTION xml_strndup
* 
* String duplicate for max N xmlChars
* INPUTS:
*   copyFrom == zero-terminated xmlChar string to copy from
*   maxlen == max number of non-zero chars to copy
* RETURNS:
*   pointer to new string or NULL if some error
*********************************************************************/
xmlChar * 
    xml_strndup (const xmlChar *copyFrom, 
		 uint32 maxlen)
{
    xmlChar *str;
    uint32  len, i;

#ifdef DEBUG
    if (!copyFrom || !maxlen) {
	return NULL;
    }
#endif

    len = min(maxlen, xml_strlen(copyFrom));

    /* get a string buffer */
    str = (xmlChar *)m__getMem(len+1);
    if (str==NULL) {
	return NULL;
    }

    /* copy the string */
    for (i=0; i<len; i++) {
	str[i] = *copyFrom++;
    }
    str[len] = 0;
    return str;

} /* xml_strndup */


/********************************************************************
* FUNCTION xml_ch_strndup
* 
* String duplicate for max N chars
* INPUTS:
*   copyFrom == zero-terminated char string to copy from
*   maxlen == max number of non-zero chars to copy
* RETURNS:
*   pointer to new string or NULL if some error
*********************************************************************/
char * 
    xml_ch_strndup (const char *copyFrom, 
		    uint32 maxlen)
{
    char *str;
    uint32  len, i;

#ifdef DEBUG
    if (!copyFrom || !maxlen) {
	return NULL;
    }
#endif

    len = min(maxlen, (uint32)strlen(copyFrom));

    /* get a string buffer */
    str = (char *)m__getMem(len+1);
    if (str==NULL) {
	return NULL;
    }

    /* copy the string */
    for (i=0; i<len; i++) {
	str[i] = *copyFrom++;
    }
    str[len] = 0;
    return str;

} /* xml_ch_strndup */


/********************************************************************
* FUNCTION xml_strcmp
* 
* String compare for xmlChar
* INPUTS:
*   s1 == zero-terminated xmlChar string to compare
*   s2 == zero-terminated xmlChar string to compare
*
* RETURNS:
*   == -1 : string 1 is less than string 2
*   == 0  : strings are equal
*   == 1  : string 1 is greater than string 2

*********************************************************************/
int 
    xml_strcmp (const xmlChar *s1, 
		const xmlChar *s2)
{
#ifdef DEBUG
    if (!s1 || !s2) {
        SET_ERROR(ERR_INTERNAL_PTR);
        return 0;
    }
#endif

    for (;;) {
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        } else if (!*s1 && !*s2) {
            return 0;
        }
        s1++;
        s2++;
    }
    /*NOTREACHED*/
}  /* xml_strcmp */


/********************************************************************
* FUNCTION xml_strncmp
* 
* String compare for xmlChar for at most 'maxlen' xmlChars
* INPUTS:
*   s1 == zero-terminated xmlChar string to compare
*   s2 == zero-terminated xmlChar string to compare
*   maxlen == max number of xmlChars to compare
*
* RETURNS:
*   == -1 : string 1 is less than string 2
*   == 0  : strings are equal
*   == 1  : string 1 is greater than string 2
*********************************************************************/
int 
    xml_strncmp (const xmlChar *s1, 
		 const xmlChar *s2,
		 uint32 maxlen)
{
    uint32  i;

#ifdef DEBUG
    if (!s1 || !s2) {
        SET_ERROR(ERR_INTERNAL_PTR);
        return 0;
    }
#endif

    for (i=0; i<maxlen; i++) {
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        } else if (!*s1 && !*s2) {
            return 0;
        }
        s1++;
        s2++;
    }
    return 0;

}  /* xml_strncmp */


/********************************************************************
* FUNCTION xml_isspace
* 
* Check if an xmlChar is a space char
* INPUTS:
*   ch == xmlChar to check
* RETURNS:
*   TRUE if a space, FALSE otherwise
*********************************************************************/
boolean
    xml_isspace (uint32 ch)
{
    char c;

    if (ch & bit7) {
        return FALSE;
    } else {
        c = (char)ch;
        return isspace(c) ? TRUE : FALSE;
    }
    /*NOTREACHED*/
} /* xml_isspace */


/********************************************************************
* FUNCTION xml_isspace_str
* 
* Check if an xmlChar string is all whitespace chars
* INPUTS:
*   str == xmlChar string to check
* RETURNS:
*   TRUE if all whitespace, FALSE otherwise
*********************************************************************/
boolean
    xml_isspace_str (const xmlChar *str)
{
#ifdef DEBUG
    if (!str) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return TRUE;
    }
#endif

    while (*str) {
	if (xml_isspace(*str)) {
	    str++;
	} else {
	    return FALSE;
	}
    }
    return TRUE;

} /* xml_isspace_str */


/********************************************************************
* FUNCTION xml_trim_string
* 
* Get string pointer + length of real string
* Get rid of the leading and trailing whitespace
*
* INPUTS:
*   str == xmlChar string to check
* OUTPUTS:
*   *len == length of the trimmed string
* RETURNS:
*   pointer to new start of string
*********************************************************************/
const xmlChar *
    xml_trim_string (const xmlChar *str, 
		     uint32 *len)
{
    const xmlChar *s2;

#ifdef DEBUG
    if (!str || !len) {
	SET_ERROR(ERR_INTERNAL_PTR);
        return NULL;
    }
#endif

    if (!*str) {
	*len = 0;
        return str;
    }

    /* str is at least char long, save ptr to start */
    s2 = str;   

    /* skip over the begin whitespace */
    while (xml_isspace(*str)) {
        str++;
    }
    if (!*str) {
        /* found only whitespace */
        *len = 0;  
        return str;
    }

    /* found at least one non-whitespace char;
     * str now points to start of real string;
     * get s2 to point to the EO string char in str 
     * then backup to the first non-whitespace
     */
    while (*s2) {
        s2++;
    }
    s2--;
    while (xml_isspace(*s2)) {
        s2--;
    }

    /* s2 now points to the last real char in the string */
    *len = (uint32)(s2-str+1);
    return str;

} /* xml_trim_string */


/********************************************************************
* FUNCTION xml_copy_clean_string
* 
* Get a malloced string contained the converted string
* from the input
* Get rid of the leading and trailing whilespace
* Get rid of character entities
* INPUTS:
*   str == xmlChar string to check
* RETURNS:
*   pointer to new malloced string
*********************************************************************/
xmlChar *
    xml_copy_clean_string (const xmlChar *str)
{
    const xmlChar *newstart;
    xmlChar *newstr, *cur;
    uint32   len, i;

#ifdef DEBUG
    if (!str) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    newstart = xml_trim_string(str, &len);

    /* malloc this full amount, which will be too big if
     * there are character entities in the string
     */
    newstr = (xmlChar *)m__getMem(len+1);
    if (!newstr) {
	SET_ERROR(ERR_INTERNAL_MEM);
	return NULL;
    }

    if (!newstart || !len) {
	*newstr = 0;
	return newstr;
    }

    /* newstart is the copy-from string
     * cur is the copy-to string
     */
    cur = newstr;
    for (i=0; i<len; ) {
	if (*newstart == '&') {
	    *cur++ = xml_convert_char_entity(newstart, &len);
	    newstart += len;
	    i += len;
	} else {
	    *cur++ = *newstart++;
	    i++;
	}
    }
    *cur = 0;
    return newstr;

} /* xml_copy_clean_string */


/********************************************************************
* FUNCTION xml_convert_char_entity
* 
* Convert an XML character entity into a single xmlChar
*
* INPUTS:
*    str == string pointing to start of char entity
* OUTPUTS:
*    *used == number of chars consumed 
* RETURNS:
*   converted xmlChar
*********************************************************************/
xmlChar
    xml_convert_char_entity (const xmlChar *str, 
			     uint32 *used)
{
    xmlChar buff[MAX_CHAR_ENT];
    uint32 i;

#ifdef DEBUG
    if (!str || !used) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return (xmlChar)' ';   /* error -- return blank */
    }
#endif

    /* make sure the start is okay */
    if (*str != '&') {
	*used = 1;
	return *str;
    } else {
	str++;
    }

    /* get the char entity name */
    i = 0;
    while (*str && (*str != ';') && (i<MAX_CHAR_ENT)) {
	buff[i++] = *str++;
    }
    buff[i] = 0;
    *used = i+2;

    /* look up the name in the conversion table */
    i = 0;
    while (chcvt[i].chstr) {
	if (!xml_strcmp(chcvt[i].chstr, buff)) {
	    return chcvt[i].ch;
	}
	i++;
    }
    
    /* not found */
    *used = 1;
    return '&';

} /* xml_convert_char_entity */


/********************************************************************
* FUNCTION xml_check_ns
*
* INPUTS:
*   reader == XmlReader already initialized from File, Memory,
*             or whatever
*   pfs->ns_id == element namespace to check
*   elname == element name to check
*
* OUTPUTS:
*   *id == namespace ID found or 0 if none
*   *pfix_len == filled in > 0 if one found
*             real element name will start at pfix_len+1
*             if pfix is non-NULL
*   *badns == pointer to unknown namespace if error returned
* RETURNS:
*   status; could be error if namespace specified but not supported
*********************************************************************/
status_t
    xml_check_ns (xmlTextReaderPtr reader,
		  const xmlChar   *elname,
		  xmlns_id_t       *id,
		  uint32           *pfix_len,
		  const xmlChar   **badns)
{
    const xmlChar *str;
    xmlns_t  *ns;

#ifdef DEBUG
    if (!reader || !elname || !id || !pfix_len || !badns) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    *id = 0;
    *pfix_len = 0;
    *badns = NULL;

    /* check for a prefix in the element namestring */
    str = elname;
    while (*str && *str != XMLNS_SEPCH) {
        str++;
    }
    if (*str) {
        /* found a prefix */
        *pfix_len = (uint32)(str-elname+1);
    }

    /* check the namespace associated with this node */
    str = xmlTextReaderNamespaceUri(reader);
    if (str && *str) {
        ns = def_reg_find_ns(str);
        if (ns) {
            *id = ns->ns_id;
        } else {
	    *id = xmlns_inv_id();
	    *badns = str;
            return ERR_NCX_UNKNOWN_NS;
        }
    }  /* else treat as no namespace, no error */

    return NO_ERR;

} /* xml_check_ns */


/* END xml_util.c */

