#!/bin/sh

kopia=/usr/local/bin/kopia
configfilename=kopiaconfig

# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $kopia
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $kopia --version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    tempdir=$(mktemp -d -t backupbouncer-90-kopia-.XXXXXXXXXX)
    echo cd $tempdir
    cd $tempdir

    touch $configfilename
    configpath="$tempdir/$configfilename"

    echo init kopia repo create
    $kopia repository create --config-file=$configpath --password=hello filesystem --path $tempdir

    echo update kopia backup policies
    $kopia --config-file=$configpath --password=hello policy set --global --ignore-dir-errors=true 
    $kopia --config-file=$configpath --password=hello policy set --global --ignore-file-errors=true 
    # WIP trying to get sudo working 
    #$kopia --config-file=$configpath --password=hello policy set --global --add-ignore="90-fifo/"
    #$kopia --config-file=$configpath --password=hello policy set --global --add-ignore="95-devices/"

    echo make kopia backup of Src
    $kopia snapshot create --config-file=$configpath --password=hello $1

    echo grab kopia objectid of last snapshot
    echo 
    objectid="$("$kopia" snapshot list --password=hello --config-file="$configpath" --max-results=1 | sed -n 2p | awk '{ print $4 }')"
    echo $objectid

    echo restore data from kopia backup to Dst
    $kopia restore --config-file=$configpath --password=hello $objectid $2

    returnvalue=$?
    return $returnvalue
}