#!/bin/bash
echo `pwd`
export PATH=$PATH:/usr/local/bin
browserify --bare $PROJECT_DIR/nodeapp/src/app.js > $PROJECT_DIR/nodeapp/www/main.js
cp $PROJECT_DIR/nodeapp/src/index.html  $PROJECT_DIR/nodeapp/www/index.html
cp $PROJECT_DIR/nodeapp/src/main.css  $PROJECT_DIR/nodeapp/www/main.css
