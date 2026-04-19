#!/bin/sh

ourpwd=$PWD

cd $HOME

git clone https://github.com/Adock90/linrecopy > /dev/null

cd linrecopy/

chmod +x ./build.sh

./build.sh

cd $ourpwd
