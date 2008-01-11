dnl Check how to export all functions, which is needed for libglade to set up
dnl the event handlers.
AC_DEFUN([CHECK_EXPORT_DYNAMIC],
[
  AC_MSG_CHECKING(for extra flags needed to export symbols)
  case $host_os in
    aix4*|aix5*)
      EXPORT_DYNAMIC_FLAGS="-Wl,-bexpall,-brtl"
      ;;
    bsdi*)
      EXPORT_DYNAMIC_FLAGS="-rdynamic"
      ;;
    *)
      save_ldflags="$LDFLAGS"
      LDFLAGS=-Wl,-export-dynamic
      AC_TRY_LINK(,,[EXPORT_DYNAMIC_FLAGS="-Wl,-export-dynamic"],
        [
          LDFLAGS=-Wl,-Bexport
          AC_TRY_LINK(,,[EXPORT_DYNAMIC_FLAGS="- Wl,-Bexport"],
                      AC_MSG_RESULT(none))
        ]
      )
      LDFLAGS="$save_ldflags"
      ;;
  esac
  AC_SUBST(EXPORT_DYNAMIC_FLAGS)
  test "x$EXPORT_DYNAMIC_FLAGS" != x && AC_MSG_RESULT([$EXPORT_DYNAMIC_FLAGS])
])
