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
  | Author: Dmitry Koterov                                               |
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

/* {{{ arginfo_dom_varimport[] */
ZEND_BEGIN_ARG_INFO_EX(arginfo_dom_varimport, 0, 0, 2)
    ZEND_ARG_OBJ_INFO(0, doc, DOMNode, 0)
    ZEND_ARG_INFO(0, var)
    ZEND_ARG_INFO(0, root_element)
    ZEND_ARG_INFO(0, badname_element)
    ZEND_ARG_INFO(0, key_attr)
    ZEND_ARG_INFO(0, notices_on_import_error)
ZEND_END_ARG_INFO();
/* }}} */

/* {{{ dom_varimport_deps[] */
#if ZEND_MODULE_API_NO >= 20050922
static const zend_module_dep dom_varimport_deps[] = {
    ZEND_MOD_REQUIRED("dom")
    ZEND_MOD_END
};
#endif
/* }}} */

/* {{{ dom_varimport_functions[] */
const zend_function_entry dom_varimport_functions[] = {
    PHP_FE(dom_varimport, arginfo_dom_varimport)
    PHP_FE_END  /* Must be the last line in dom_varimport_functions[] */
};
/* }}} */

/* {{{ dom_varimport_module_entry */
zend_module_entry dom_varimport_module_entry = {
    STANDARD_MODULE_HEADER_EX, NULL,
    dom_varimport_deps,
    "dom_varimport",
    dom_varimport_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(dom_varimport),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_DOM_VARIMPORT_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DOM_VARIMPORT
ZEND_GET_MODULE(dom_varimport)
#endif

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(dom_varimport)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "dom_varimport support", "enabled");
    php_info_print_table_row(2, "Version", PHP_DOM_VARIMPORT_VERSION);
    php_info_print_table_end();
}
/* }}} */

/* dom_varimport() settings (function arguments). */
typedef struct _dom_varimport_config {
    char *root_element_name;
    char *badname_element_name;
    char *key_attr_name;
    zend_bool notices_on_import_error;
} dom_varimport_config;

/* Forward declaration for recursion. */
static void php_dom_varimport(xmlNodePtr node, zval *val, dom_varimport_config *conf, char *cur_key TSRMLS_DC);

static int php_is_valid_tag_name(char *s) /* {{{ */
{
    return xmlValidateName((xmlChar *)s, 0) == 0;
}
/* }}} */

static void php_dom_varimport_array(xmlNodePtr node, zval **val, dom_varimport_config *conf TSRMLS_DC) /* {{{ */
{
    HashTable *ht = HASH_OF(*val);

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
        xmlNodePtr e;

        zend_hash_internal_pointer_reset_ex(ht, &pos);
        for (;; zend_hash_move_forward_ex(ht, &pos)) {
            i = zend_hash_get_current_key_ex(ht, &key, &key_len, &index, 0, &pos);
            if (i == HASH_KEY_NON_EXISTANT)
                break;

            if (zend_hash_get_current_data_ex(ht, (void **)&data, &pos) == SUCCESS) {
                HashTable *tmp_ht = HASH_OF(*data);
                if (tmp_ht != NULL) {
                    tmp_ht->nApplyCount++;
                }

                tag = conf->badname_element_name;

                if (i == HASH_KEY_IS_STRING) {
                    if (key[0] == '\0' && Z_TYPE_PP(val) == IS_OBJECT) {
                        /* Skip protected and private members. */
                        if (tmp_ht != NULL) {
                            tmp_ht->nApplyCount--;
                        }
                        continue;
                    }
                    if (strcmp(key, "@attributes") == 0) {
                        HashTable *arr_ht = HASH_OF(*data);
                        if (arr_ht != NULL) {
                            HashPosition arr_pos;
                            zval **arr_data;
                            char *arr_key;
                            ulong arr_index;
                            uint arr_key_len;
                            int j;

                            zend_hash_internal_pointer_reset_ex(arr_ht, &arr_pos);
                            for (;; zend_hash_move_forward_ex(arr_ht, &arr_pos)) {
                                j = zend_hash_get_current_key_ex(arr_ht, &arr_key, &arr_key_len, &arr_index, 0, &arr_pos);

                                if (j == HASH_KEY_NON_EXISTANT)
                                    break;

                                if (j != HASH_KEY_IS_STRING) {
                                    if (conf->notices_on_import_error) {
                                        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Passed a numeric attribute name, skipping");
                                    }
                                    continue;
                                }

                                if (arr_key[0] == '\0' && Z_TYPE_PP(data) == IS_OBJECT) {
                                    /* Skip protected and private members. */
                                    continue;
                                }

                                if (zend_hash_get_current_data_ex(arr_ht, (void**)&arr_data, &arr_pos) == SUCCESS) {
                                    if (!php_is_valid_tag_name(arr_key)) {
                                        if (conf->notices_on_import_error) {
                                            php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Passed invalid attribute name, skipping");
                                        }
                                        continue;
                                    }
                                    switch (Z_TYPE_P(*arr_data)) {
                                        case IS_NULL:
                                            break;

                                        case IS_BOOL:
                                            if (Z_BVAL_P(*arr_data)) {
                                                xmlNewProp(node, BAD_CAST arr_key, BAD_CAST "true");
                                            } else {
                                                xmlNewProp(node, BAD_CAST arr_key, BAD_CAST "false");
                                            }
                                            break;

                                        case IS_LONG:
                                        case IS_DOUBLE:
                                            convert_to_string(*arr_data);
                                        case IS_STRING:
                                            xmlNewProp(node, BAD_CAST arr_key, BAD_CAST Z_STRVAL_P(*arr_data));
                                            break;

                                        case IS_ARRAY:
                                        case IS_OBJECT:
                                        case IS_RESOURCE:
                                            if (conf->notices_on_import_error) {
                                                php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Invalid attribute value: array/resource/object, attribute: \"%s\"", arr_key);
                                            }
                                            break;

                                        default:
                                            if (conf->notices_on_import_error) {
                                                php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Unsupported attribute value: unknown, attribute: \"%s\"", arr_key);
                                            }
                                            break;
                                    }
                                }
                            }
                        } else {
                            if (conf->notices_on_import_error) {
                                php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Addtibutes must be specified as an array or object");
                            }
                        }
                        continue; /* finish parsing @attributes */
                    }
                    if (php_is_valid_tag_name(key)) {
                        tag = key;
                    }
                } else {
                    sprintf(buf, "%ld", (long)index);
                    key = buf;
                }

                if (tag != NULL) {
                    if (conf->notices_on_import_error && tag == conf->badname_element_name) {
                        php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Array key or object property \"%s\" cannot be used as XML element name; \"%s\" is used instead", key, tag);
                    }
                    e = xmlNewChild(node, NULL, BAD_CAST tag, NULL);
                    if (conf->key_attr_name != NULL) {
                        xmlNewProp(e, BAD_CAST conf->key_attr_name, BAD_CAST key);
                    }
                    php_dom_varimport(e, *data, conf, key TSRMLS_CC);
                }

                if (tmp_ht != NULL) {
                    tmp_ht->nApplyCount--;
                }
            }
        }
    }
}
/* }}} */

static void php_dom_varimport(xmlNodePtr node, zval *val, dom_varimport_config *conf, char *cur_key TSRMLS_DC) /* {{{ */
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
            php_dom_varimport_array(node, &val, conf TSRMLS_CC);
            break;

        case IS_RESOURCE:
            if (conf->notices_on_import_error) {
                php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Unsupported value type: resource, key: \"%s\"", cur_key);
            }
            break;

        default:
            if (conf->notices_on_import_error) {
                php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Unsupported value type: unknown, key: \"%s\"", cur_key);
            }
            break;
    }

    if (text != NULL) {
        xmlAddChild(node, text);
    }
}
/* }}} */

/* {{{ proto string dom_varimport(DOMDocument doc, mixed var [, string root_element = "root" [, string badname_element = "item" [, string key_attr = "key" [, bool notices_on_import_error = false]]]])
   Assigns serialized var content to doc root element named root_element (defaults to "root").
   Array keys or object properties become XML element names. If such name is invalid for XML,
   badname_element is used instead (or, if badname_element is NULL, it is skipped). Anyway,
   original array key or object property name is stored in XML element's attribute named key_attr
   (if you pass NULL in key_attr, no such attribute is created). If notices_on_import_error is
   set to true, notices are generated when badname_element is used instead of an XML-invalid
   element name. */
PHP_FUNCTION(dom_varimport)
{
    zval *id, *var;
    dom_varimport_config conf;
    int dummy = 0;
    xmlNodePtr nodep = NULL;
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, old_root_node;

    memset(&conf, 0, sizeof(conf));
    conf.root_element_name = PHP_DOM_VARIMPORT_DEFAULT_ROOT_ELEMENT_NAME;
    conf.badname_element_name = PHP_DOM_VARIMPORT_DEFAULT_BADNAME_ELEMENT_NAME;
    conf.key_attr_name = PHP_DOM_VARIMPORT_DEFAULT_KEY_ATTR_NAME;
    conf.notices_on_import_error = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "oz|ss!s!b", &id, &var, &conf.root_element_name, &dummy, &conf.badname_element_name, &dummy, &conf.key_attr_name, &dummy, &conf.notices_on_import_error) == FAILURE) {
        return;
    }

    if (!php_is_valid_tag_name(conf.root_element_name)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed invalid root_element \"%s\"; default value \"%s\" is used instead", conf.root_element_name, PHP_DOM_VARIMPORT_DEFAULT_ROOT_ELEMENT_NAME);
        conf.root_element_name = PHP_DOM_VARIMPORT_DEFAULT_ROOT_ELEMENT_NAME; /* for NULL sets default value as well */
    }

    if (conf.badname_element_name != NULL && !php_is_valid_tag_name(conf.badname_element_name)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed invalid badname_element \"%s\"; default value \"%s\" is used instead", conf.badname_element_name, PHP_DOM_VARIMPORT_DEFAULT_BADNAME_ELEMENT_NAME);
        conf.badname_element_name = PHP_DOM_VARIMPORT_DEFAULT_BADNAME_ELEMENT_NAME;
    }

    if (conf.key_attr_name != NULL && !php_is_valid_tag_name(conf.key_attr_name)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed invalid key_attr \"%s\"; default value \"%s\" is used instead", conf.key_attr_name, PHP_DOM_VARIMPORT_DEFAULT_KEY_ATTR_NAME);
        conf.key_attr_name = PHP_DOM_VARIMPORT_DEFAULT_KEY_ATTR_NAME;
    }

    nodep = php_libxml_import_node(id TSRMLS_CC);

    if (nodep != NULL) {
        doc = nodep->doc;
    }

    if (doc == NULL) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed invalid DOMDocument");
        RETURN_FALSE;
    }

    if (nodep->type == XML_DOCUMENT_NODE) {
        root_node = xmlNewNode(NULL, BAD_CAST conf.root_element_name);
        old_root_node = xmlDocSetRootElement(doc, root_node);
        if (old_root_node != NULL) {
            xmlUnlinkNode(old_root_node);
            xmlFreeNode(old_root_node);
        }
    } else if (nodep->type == XML_ELEMENT_NODE) {
        root_node = nodep;
    } else {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Passed invalid DOMDocument or DOMElement");
        RETURN_FALSE;
    }

    php_dom_varimport(root_node, var, &conf, "(variable itself)" TSRMLS_CC);
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
