AC_INIT([vector], [0.2], [carlos.caronte@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE([ -Wall -Werror])
AM_PATH_CHECK()
AM_PROG_AR
LT_INIT()
CFLAGS='-g '

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT

