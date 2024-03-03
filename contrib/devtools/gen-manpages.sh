#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

AIDPD=${AIDPD:-$SRCDIR/aidpd}
AIDPCLI=${AIDPCLI:-$SRCDIR/aidp-cli}
AIDPTX=${AIDPTX:-$SRCDIR/aidp-tx}
AIDPQT=${AIDPQT:-$SRCDIR/qt/aidp-qt}

[ ! -x $AIDPD ] && echo "$AIDPD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
AIDPVER=($($AIDPCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for aidpd if --version-string is not set,
# but has different outcomes for aidp-qt and aidp-cli.
echo "[COPYRIGHT]" > footer.h2m
$AIDPD --version | sed -n '1!p' >> footer.h2m

for cmd in $AIDPD $AIDPCLI $AIDPTX $AIDPQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${AIDPVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${AIDPVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
