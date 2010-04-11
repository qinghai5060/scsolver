#!/bin/bash

AUTHORNAME="Kohei Yoshida"
AUTHOREMAIL=kohei.yoshida@gmail.com
STATUSFILE=/tmp/filelist.status
DATE=$(date +%F)
svn status | sort > $STATUSFILE

cat $STATUSFILE | grep -E "^A\ \ \ " > $STATUSFILE-added
cat $STATUSFILE | grep -E "^A\ \ \+" > $STATUSFILE-moved
cat $STATUSFILE | grep -E "^M\ \ " > $STATUSFILE-modified
cat $STATUSFILE | grep -E "^D\ \ " > $STATUSFILE-deleted

echo "$DATE  $AUTHORNAME  <$AUTHOREMAIL>"
echo

TEMP=$(cat $STATUSFILE-added | wc -l)
if [ ! $TEMP == 0 ]; then
    echo added
    cat $STATUSFILE-added | sed \
	-e "s/^A\ */\t*\ /g" \
	-e "s/$/:/g"
fi

TEMP=$(cat $STATUSFILE-moved | wc -l)
if [ ! $TEMP == 0 ]; then
    echo moved
    cat $STATUSFILE-moved | sed \
	-e "s/^A\ \ +\ */\t* /g" \
	-e "s/$/:/g"
fi

TEMP=$(cat $STATUSFILE-modified | wc -l)
if [ ! $TEMP == 0 ]; then
    echo modified
    cat $STATUSFILE-modified | sed \
	-e "s/^M\ */\t*\ /g" \
	-e "s/$/:/g"
fi

TEMP=$(cat $STATUSFILE-deleted | wc -l)
if [ ! $TEMP == 0 ]; then
    echo deleted
    cat $STATUSFILE-deleted | sed \
	-e "s/^D\ */\t*\ /g" \
	-e "s/$/:/g"
fi

    
