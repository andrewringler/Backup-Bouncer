#!/bin/sh

restic=$HOME/bin/restic
if [[ ! -e $restic ]]
then
restic=/usr/local/bin/restic
fi

# flags="-aH -E --rsync-path=$rsync"
# This should resemble what we use with mlbackup as closely as possible.


# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $restic
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $restic version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    restictempdir=$(mktemp -d -t backupbouncer-85-restic-.XXXXXXXXXX)

    echo init restic repo
    RESTIC_PASSWORD="test" "$restic" init --repo "$restictempdir"
    echo make restic backup of Src
    cd $1
    RESTIC_PASSWORD="test" "$restic" --repo "$restictempdir" backup .
    echo restore data from restic backup to Dst
    RESTIC_PASSWORD="test" "$restic" --repo "$restictempdir" restore latest --target $2
    returnvalue=$?
    echo clean up temp directory
    unset RESTIC_PASSWORD
    return $returnvalue
}
