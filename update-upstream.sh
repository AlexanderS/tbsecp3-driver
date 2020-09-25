#!/bin/sh
TBS_DIR="$(cd "$1" >/dev/null; pwd)"

if [ ! -d "$TBS_DIR" ] || [ ! -d "$TBS_DIR/.git" ] || [ ! -r "$TBS_DIR/drivers/media/pci/tbsecp3/Makefile" ]; then
    echo "You need to give the path to the updated upstream sources" >&2
    echo "(the media tree) as first argument." >&2
    exit 1
fi


cd "$(dirname $0)"
if [ -n "$(git status -s)" ]; then
    echo "You have uncommited changes. Maybe you want to 'make clean'." >&2
    git status -s
    exit 1
fi

find tbsecp3/ -name '*.[ch]' -type f -print0 | while IFS= read -r -d $'\0' file; do
    cp "$TBS_DIR/drivers/media/pci/$file" "$file"
done

find saa716x/ -name '*.[ch]' -type f -print0 | while IFS= read -r -d $'\0' file; do
    cp "$TBS_DIR/drivers/media/pci/$file" "$file"
done

find dvb-frontends/ tuners/ -name '*.[ch]' -type f -print0 | while IFS= read -r -d $'\0' file; do
    cp "$TBS_DIR/drivers/media/$file" "$file"
done

if [ -n "$(git status -s)" ]; then
    git add .

    commit=$(git --git-dir "$TBS_DIR/.git/" rev-parse HEAD)
    git commit -F- <<EOF
Update upstream version
    
This is from commit: $commit
EOF
fi
