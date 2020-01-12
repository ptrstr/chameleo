#!/bin/sh
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR

git remote add origin https://github.com/ptrstr/stegano.git
git pull origin master
