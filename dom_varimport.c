/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_dom_varimport.h"
#include "ext/libxml/php_libxml.h"


ZEND_BEGIN_ARG_INFO_EX(arginfo_dom_varimport, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, doc, DOMDocument, 0)
    ZEND_ARG_INFO(0, var)
    ZEND_ARG_INFO(0, root_element)
ZEND_END_ARG_INFO();

/* True global resources - no need for thread safety here */
static int le_dom_varimport;

/* {{{ dom_varimport_functions[]
 *
 * Every user visible function must have an entry in dom_varimport_functions[].
 */
const zend_function_entry dom_varimport_functions[] = {
    PHP_FE(dom_varimport, arginfo_dom_varimport)
    PHP_FE_END  /* Must be the last line in dom_varimport_functions[] */
};
/* }}} */

/* {{{ dom_varimport_module_entry
 */
zend_module_entry dom_varimport_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "dom_varimport",
    dom_varimport_functions,
    PHP_MINIT(dom_varimport),
    PHP_MSHUTDOWN(dom_varimport),
    PHP_RINIT(dom_varimport),       /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(dom_varimport),   /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(dom_varimport),
#if ZEND_MODULE_API_NO >= 20010901
    "1.02",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DOM_VARIMPORT
ZEND_GET_MODULE(dom_varimport)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(dom_varimport)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(dom_varimport)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(dom_varimport)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(dom_varimport)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(dom_varimport)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "dom_varimport support", "enabled");
    php_info_print_table_end();
}
/* }}} */

static int php_is_valid_tag_name(char *s)
{
    if (isalpha(*s) || *s == '_') {
        ++s;
        while (*s) {
            if (!isalnum(*s) && *s != '_' && *s != '-' && *s != '.') return 0;
            ++s;
        }
        return 1;
    }
    return 0;
}

static void php_dom_varimport(xmlNodePtr node, zval *val);

static void php_dom_varimport_array(xmlNodePtr node, zval **val) /* {{{ */
{
    HashTable *ht;

    if (Z_TYPE_PP(val) == IS_ARRAY) {
        ht = HASH_OF(*val);
    } else {
        ht = Z_OBJPROP_PP(val);
    }

    if (ht == NULL) {
        return;
    }

    if (ht->nApplyCount > 1) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "recursion detected");
        return;
    }

    if (zend_hash_num_elements(ht) > 0) {
        HashPosition pos;
        int i;
        char *key, *tag, buf[128];
        zval **data;
        ulong index;
        uint key_len;
        xmlNodePtr text;
        xmlNodePtr e;

        zend_hash_internal_pointer_reset_ex(ht, &pos);
        for (;; zend_hash_move_forward_ex(ht, &pos)) {
            i = zend_hash_get_current_key_ex(ht, &key, &key_len, &index, 0, &pos);
            if (i == HASH_KEY_NON_EXISTANT)
                break;

            if (zend_hash_get_current_data_ex(ht, (void **)&data, &pos) == SUCCESS) {
                HashTable *tmp_ht = HASH_OF(*data);
                if (tmp_ht) {
                    tmp_ht->nApplyCount++;
                }

                tag = "item";

                if (i == HASH_KEY_IS_STRING) {
                    if (key[0] == '\0' && Z_TYPE_PP(val) == IS_OBJECT) {
                        /* Skip protected and private members. */
                        if (tmp_ht) {
                            tmp_ht->nApplyCount--;
                        }
                        continue;
                    }
                    if (php_is_valid_tag_name(key)) {
                        tag = key;
                    }
                } else {
                    sprintf(buf, "%ld", (long)index);
                    key = buf;
                }

                e = xmlNewChild(node, NULL, BAD_CAST tag, NULL);
                xmlNewProp(e, BAD_CAST "key", BAD_CAST key);
                php_dom_varimport(e, *data);

                if (tmp_ht) {
                    tmp_ht->nApplyCount--;
                }
            }
        }
    }
}

static void php_dom_varimport(xmlNodePtr node, zval *val) /* {{{ */
{
    xmlNodePtr text = NULL;
    int len;
    char buf[128];
    double dbl;

    switch (Z_TYPE_P(val))
    {
        case IS_NULL:
            break;

        case IS_BOOL:
            if (Z_BVAL_P(val)) {
                text = xmlNewTextLen(BAD_CAST "1", 1);
            } else {
                // Nothing - "false" is an empty string, result is an empty node.
            }
            break;

        case IS_LONG:
            len = sprintf(buf, "%ld", Z_LVAL_P(val));
            text = xmlNewTextLen(BAD_CAST buf, len);
            break;

        case IS_DOUBLE:
            dbl = Z_DVAL_P(val);
            if (!zend_isinf(dbl) && !zend_isnan(dbl)) {
                len = snprintf(buf, sizeof(buf), "%.*k", (int)EG(precision), dbl);
            } else {
                len = sprintf(buf, "%f", dbl);
            }
            text = xmlNewTextLen(BAD_CAST buf, len);
            break;

        case IS_STRING:
            text = xmlNewTextLen(BAD_CAST Z_STRVAL_P(val), Z_STRLEN_P(val));
            break;

        case IS_ARRAY:
        case IS_OBJECT:
            php_dom_varimport_array(node, &val);
            break;

        default:
            break;
    }

    if (text != NULL) {
        xmlAddChild(node, text);
    }
}
/* }}} */

/* {{{ proto string dom_varimport(DOMDocument doc, mixed var [, string root_element])
   Assigns serialized var content to doc root element named root_element (defaults to "root"). */
PHP_FUNCTION(dom_varimport)
{
    zval *id, *var;
    char *root_element_name = NULL;
    int root_element_len = 0;
    xmlNodePtr nodep = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, old_root_node;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oz|s", &id, &var, &root_element_name, &root_element_len) == FAILURE) {
        return;
    }

    if (!root_element_name || !php_is_valid_tag_name(root_element_name)) {
        root_element_name = "root";
    }

    nodep = php_libxml_import_node(id TSRMLS_CC);

    if (nodep) {
        doc = nodep->doc;
    }
    if (doc == NULL) {
        php_error(E_WARNING, "Invalid Document");
        RETURN_FALSE;
    }

    root_node = xmlNewNode(NULL, BAD_CAST root_element_name);
    old_root_node = xmlDocSetRootElement(doc, root_node);
    if (old_root_node != NULL) {
        xmlUnlinkNode(old_root_node);
        xmlFreeNode(old_root_node);
    }

    php_dom_varimport(root_node, var);
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
