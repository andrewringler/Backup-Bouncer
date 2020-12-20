#!/bin/sh

borg=/usr/local/bin/borg

# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $borg
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $borg --version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    tempdir=$(mktemp -d -t backupbouncer-130-borg-.XXXXXXXXXX)

    echo create borg repository
    cd $tempdir
    BORG_RELOCATED_REPO_ACCESS_IS_OK=yes $borg init -e none repo 

    echo backup files to borg repo
    cd $1
    BORG_RELOCATED_REPO_ACCESS_IS_OK=yes $borg create $tempdir/repo::TheBackup .

    echo restore data from borg backup to Dst
    cd $2
    sudo BORG_RELOCATED_REPO_ACCESS_IS_OK=yes $borg extract $tempdir/repo::TheBackup

    unset BORG_RELOCATED_REPO_ACCESS_IS_OK
    returnvalue=$?
    return $returnvalue
}