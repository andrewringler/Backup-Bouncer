#!/bin/sh

gitannex=/Applications/git-annex.app/Contents/MacOS/git-annex
git=/Applications/git-annex.app/Contents/MacOS/git

# Should exit with code 0 if the necessary programs exist, 1 otherwise
can-copy () {
    test -e $gitannex
}

# Should generate some text on stdout identifying which version of the
# copier is being used, and how it's called.  This is optional.
version () {
    $gitannex version
}

# Should perform a copy from $1 to $2.  Both will be directories.  Neither
# will end with '/'.  So you'll get something like:
#   backup /Volumes/Src /Volumes/Dst/99-foo
backup () {
    gitannextempdir=$(mktemp -d -t backupbouncer-100-git-annex-.XXXXXXXXXX)
    cd $gitannextempdir

    echo init git-annex repo
    $git init
    $gitannex init

    echo import files into git-annex repo
    $gitannex import --duplicate $1/
    $gitannex add .
    $git commit -m "added source files"

    echo create another git-annex repo at destination and get all files
    cd $2
    $git init
    $gitannex init
    $git remote add source $gitannextempdir
    $gitannex sync
    $gitannex get .
    $gitannex unlock .
    mv Src/* .

    returnvalue=$?
    return $returnvalue
}