#!/bin/bash

SDFPATH=/home/kyoshida/ooo/ooo-build/trunk/build/current/sc/source/ui/src/localize.sdf

cat $SDFPATH | grep SID_OPENDLG_OPTSOLVER | sed \
    -e 's/.*SID_OPENDLG_OPTSOLVER\t*230\t/<value xml:lang="/g' \
    -e 's/\t*2002.*$/...<\/value>/g' | sed -e 's/\t/">/g'
