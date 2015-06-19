#!/bin/bash
echo `pwd`
export PATH=$PATH:/usr/local/bin
cd $PROJECT_DIR/nodeapp/
jx package $PROJECT_DIR/nodeapp/src/app.js main
mv $PROJECT_DIR/nodeapp/main.jx $PROJECT_DIR/nodeapp/www/main.jx
