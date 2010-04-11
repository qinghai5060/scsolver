#!/bin/bash

DATE=$(date +%Y-%m-%d)
svn checkout http://scsolver.googlecode.com/svn/trunk/ scsolver || exit 1
find ./scsolver -name '.svn' -type d | xargs rm -r || exit 1
pushd . && cd scsolver/ext && autoconf && rm -rf autom4te.cache autogen.sh && popd || exit 1
tar jcvf scsolver.$DATE.tar.bz2 scsolver



