#!/bin/sh

rm -rf dist
mkdir -p dist
cd dist

ln -s "../build/src" include
