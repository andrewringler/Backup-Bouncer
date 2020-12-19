#!/bin/sh

dar=/usr/local/bin/dar

# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $dar
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $dar --version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    tempdir=$(mktemp -d -t backupbouncer-120-dar-.XXXXXXXXXX)

    echo create dar backup
    cd $1
    sudo $dar -c $tempdir/thebackup.dar -R .

    echo restore data from dar backup to Dst
    cd $2
    sudo $dar -x $tempdir/thebackup.dar

    returnvalue=$?
    return $returnvalue
}