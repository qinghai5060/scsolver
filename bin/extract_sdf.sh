#!/bin/bash

run_convert() {
    FILE=$1
    LOCALE=$(get_locale.py "$FILE")
    LOCALE2=$(echo $LOCALE | sed -e 's/\-/\\-/g')
    
    cat "$FILE" | grep SCSOLVER | grep -E "$LOCALE2" | grep -v "en-US" | sed \
        -e 's/.*SCSOLVER/SCSOLVER/g' \
        -e "s/\t*[0-9]*\t*$LOCALE2//g" \
        -e 's/\t*200[7-8].*$//g' > $LOCALE.txt
}

while [ -e "$1" ]; do
    run_convert "$1"
    shift 1
done
