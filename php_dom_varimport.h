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

#ifndef PHP_DOM_VARIMPORT_H
#define PHP_DOM_VARIMPORT_H

#define PHP_DOM_VARIMPORT_VERSION "1.11.3"
#define PHP_DOM_VARIMPORT_DEFAULT_ROOT_ELEMENT_NAME "root"
#define PHP_DOM_VARIMPORT_DEFAULT_BADNAME_ELEMENT_NAME "item"
#define PHP_DOM_VARIMPORT_DEFAULT_KEY_ATTR_NAME "key"

extern zend_module_entry dom_varimport_module_entry;
#define phpext_dom_varimport_ptr &dom_varimport_module_entry

#ifdef PHP_WIN32
#    define PHP_DOM_VARIMPORT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_DOM_VARIMPORT_API __attribute__ ((visibility("default")))
#else
#    define PHP_DOM_VARIMPORT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(dom_varimport);
PHP_FUNCTION(dom_varimport);

#ifdef ZTS
#define DOM_VARIMPORT_G(v) TSRMG(dom_varimport_globals_id, zend_dom_varimport_globals *, v)
#else
#define DOM_VARIMPORT_G(v) (dom_varimport_globals.v)
#endif

#endif /* PHP_DOM_VARIMPORT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
