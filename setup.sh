#!/bin/sh

DIR=`dirname $0`
DIR=`cd $DIR; pwd`
ln -s "$DIR/.gitconfig" ~/
ln -s "$DIR/.gitexcludesfile" ~/
ln -s "$DIR/.gitmodules" ~/
ln -s "$DIR/.gitscreenrc" ~/
ln -s "$DIR/.screenrc" ~/
ln -s "$DIR/.vim" ~/
ln -s "$DIR/.vimrc" ~/
ln -s "$DIR/.zprofile" ~/
ln -s "$DIR/.zshrc" ~/
ln -s "$DIR/.zshrc.my" ~/

cd "$DIR/.terminfo"
tic mostlike.txt
cd -
