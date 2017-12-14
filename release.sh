#!/bin/bash
# Author Mamoru Kaminaga
# Date 2017/09/08
# Shell script for release
# Copyright 2017 Mamoru Kaminaga

if [ "$1" = "" ]; then
  echo 'need args, 1->ubuntu, 2->windows'
  exit 1
fi

# Required: The latest version.
VERSION=$(git describe --tags)

# Output directory is created
OUTDIR="release"
mkdir -p ${OUTDIR}

if [ "$1" = "1" ]; then
  # Build test for Ubuntu.
  touch *.cc
  make
  if [ $? != 0 ]; then exit 1; fi

  # Output sub directory is created
  TARGET="${OUTDIR}/${VERSION}_ubuntu"
elif [ "$1" = "2" ]; then
  # Build test for Windows (Console).
  touch *.cc
  nmake /f makefile_windows.mk | iconv -f cp932 -t utf-8
  if [ $? != 0 ]; then exit 1; fi

  # Build test for Windows (GUI).
  touch *.cc
  nmake /f makefile_windows_GUI.mk | iconv -f cp932 -t utf-8
  if [ $? != 0 ]; then exit 1; fi

  # Output sub directory is created
  TARGET="${OUTDIR}/${VERSION}_windows"
fi

rm -r ${TARGET}
mkdir -p ${TARGET}

cp *.md ${TARGET}  # descriptions
cp *.txt ${TARGET}  # descriptions
cp *.exe ${TARGET}  # Library files for Windows
cp *.out ${TARGET}  # Library files for Windows

# Zip file is created.
zip -r ${TARGET}.zip ${TARGET}

exit 0
