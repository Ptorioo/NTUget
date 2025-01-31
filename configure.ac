AC_PREREQ([2.71])

AC_INIT([ntuget], [-])

AC_CONFIG_SRCDIR([src/ntuget.c])
AC_CONFIG_FILES([Makefile include/Makefile src/Makefile])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.16 -Wall -Werror])
AM_MAINTAINER_MODE
AM_CONDITIONAL([DEBUG], [test "$debug" = "yes"])

PKG_CHECK_MODULES([DEPS], [libcurl >= 8.9])

AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [Enable debugging])],
  [debug=yes], [debug=no])

AM_CONDITIONAL([DEBUG], [test "$debug" = "yes"])

if test "$debug" = "yes"; then
  CFLAGS="-g -Wall -Wextra -Wno-unused-parameter -pedantic -fsanitize=address"
  LDFLAGS="-fsanitize=address"
else
  CFLAGS="-O2 -DNDEBUG"
  LDFLAGS="-s"
fi

AC_SUBST(CFLAGS)
AC_SUBST(LDFLAGS)
AC_SUBST([DEPS_CFLAGS])
AC_SUBST([DEPS_LIBS])

AC_PROG_CC

AC_OUTPUT
