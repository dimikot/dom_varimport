dnl
dnl $Id$
dnl

PHP_ARG_ENABLE(dom_varimport, whether to enable dom_varimport support,
[  --enable-dom_varimport             Enable dom_varimport support])

if test -z "$PHP_LIBXML_DIR"; then
  PHP_ARG_WITH(libxml-dir, libxml2 install dir,
  [  --with-libxml-dir=DIR   dom_varimport: libxml2 install prefix], no, no)
fi

if test "$PHP_DOM_VARIMPORT" != "no"; then

  if test "$PHP_LIBXML" = "no"; then
    AC_MSG_ERROR([dom_varimport extension requires LIBXML extension, add --enable-libxml])
  fi

  if test "$PHP_DOM" = "no"; then
    AC_MSG_ERROR([dom_varimport extension requires DOM extension, add --enable-dom])
  fi

  PHP_SETUP_LIBXML(DOM_VARIMPORT_SHARED_LIBADD, [
    PHP_SUBST(DOM_VARIMPORT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(dom_varimport, dom_varimport.c, $ext_shared)
    PHP_ADD_EXTENSION_DEP(dom_varimport, dom, true)
    PHP_ADD_EXTENSION_DEP(dom_varimport, libxml, true)
    PHP_INSTALL_HEADERS([ext/dom_varimport/php_dom_varimport.h])
  ], [
    AC_MSG_ERROR([xml2-config not found. Please check your libxml2 installation.])
  ])

  CFLAGS="$CFLAGS -Wall"
fi
