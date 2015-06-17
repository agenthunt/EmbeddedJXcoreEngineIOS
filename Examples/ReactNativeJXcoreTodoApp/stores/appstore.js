(function() {
  'use strict';

  var AppDispatcher = require('../dispatcher/appdispatcher.js');
  var EventEmitter = require('events').EventEmitter;
  var AppConstants = require('../constants/appconstants.js');
  var AppAPIUtils = require('../utils/appapiutils.js');
  var assign = require('object-assign');
  var Items = [];


  var AppStore = assign({}, EventEmitter.prototype, {

    getItems: function() {
      return Items;
    },

    emitChange: function() {
      this.emit(AppConstants.CHANGE);
    },

    /**
     * @param {function} callback
     */
    addChangeListener: function(callback) {
      this.on(AppConstants.CHANGE, callback);
    },

    /**
     * @param {function} callback
     */
    removeChangeListener: function(callback) {
      this.removeListener(AppConstants.CHANGE, callback);
    },

    //data and action flows from server to views
    handleServerAction: function(action) {
      //console.log('AppStore:handleReceiveAction' + JSON.stringify(action));
      switch(action.type) {
        case AppConstants.RECEIVE_ITEMS:
          Items = action.obj;
          this.emitChange();
          break;
        default:
      }
    },
    //data and action flows from views to server
    handleViewAction: function(action) {
      //console.log('AppStore:handleViewAction' + action);
      switch(action.type) {
        case AppConstants.GET_ITEMS:
          AppAPIUtils.getItems();
          break;
        case AppConstants.ADD_ITEM:
          AppAPIUtils.addItem(action.obj);
          break;
        case AppConstants.UPDATE_ITEM:
          AppAPIUtils.updateItem(action.obj);
          break;
        case AppConstants.DELETE_ITEM:
          AppAPIUtils.deleteItem(action.obj);
          break;

        default:

      }
    },
    dispatcherIndex: AppDispatcher.register(function(payload) {
      //console.log(payload);
      switch(payload.source) {
        case AppConstants.SERVER_ACTION:
          AppStore.handleServerAction(payload.action);
          break;
        case AppConstants.VIEW_ACTION:
          AppStore.handleViewAction(payload.action);
          break;
        default:
      }
    })

  });

  module.exports = AppStore;
}());
