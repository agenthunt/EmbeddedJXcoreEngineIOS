#!/bin/bash
echo `pwd`
export PATH=$PATH:/usr/local/bin
#rm $PROJECT_DIR/jxcoreapp/www/main.js
cd $PROJECT_DIR/jxcoreapp/
#webpack
jx package $PROJECT_DIR/jxcoreapp/server.js main
mv $PROJECT_DIR/jxcoreapp/main.jx $PROJECT_DIR/jxcoreapp/www/main.jx
