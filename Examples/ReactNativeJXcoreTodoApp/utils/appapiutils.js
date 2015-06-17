(function() {
  'use strict';
  var AppActions = require('../actions/appactions.js');
  var AppConstants = require('../constants/appconstants.js');
  var request = require('superagent');
  var REQUEST_URL = 'http://localhost:8080/api/items';
  var AppAPIUtils = {
    getItems: function() {
      request.get(REQUEST_URL)
        .end(function(err, res) {
          if (!err)
            AppActions.receiveItems(res.body);
          else
            throw err;
        });
    },
    addItem: function(obj) {
      request.post(REQUEST_URL)
        .type('form')
        .send(obj)
        .end(function(err, res) {
          if (!err)
            AppActions.receiveItems(res.body);
          else
            throw err;
        });
    },
    updateItem: function(obj) {
      console.log('updateItem' + JSON.stringify(obj));
      request.put(REQUEST_URL + '/' + obj._id)
        .set('Content-Type', 'application/json')
        .send(obj)
        .end(function(err, res) {
          if (!err)
            AppActions.receiveItems(res.body);
          else 
            throw err;
        });
    },
    deleteItem: function(obj) {
      console.log('deleteItem' + JSON.stringify(obj));
      request.del(REQUEST_URL + '/' + obj._id)
        .set('Content-Type', 'application/json').
        send(obj)
        .end(function(err, res) {
          if (!err)
            AppActions.receiveItems(res.body);
          else
            throw err;
        });
    }
  };
  module.exports = AppAPIUtils;
}());