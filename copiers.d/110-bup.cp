#!/bin/sh

bup=/usr/local/bin/bup

# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $bup
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $bup version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    tempdir=$(mktemp -d -t backupbouncer-85-restic-.XXXXXXXXXX)

    echo init bup repo
    cd $tempdir
    $bup -d $tempdir init

    echo backup src to bup repo
    sudo $bup -d $tempdir index $1
    sudo $bup -d $tempdir save -n local-src $1

    echo restore data from bup backup to Dst
    sudo $bup -d $tempdir restore --outdir=$2 local-src/latest/$1/

    returnvalue=$?
    echo clean up temp directory
    unset RESTIC_PASSWORD
    return $returnvalue
}
