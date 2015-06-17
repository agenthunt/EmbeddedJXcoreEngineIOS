(function() {
  'use strict';

  var Dispatcher = require('flux').Dispatcher,
    AppConstants = require('../constants/appconstants.js'),
    assign = require('object-assign'),
    AppDispatcher;


  AppDispatcher = assign(new Dispatcher(), {
    handleServerAction: function(action) {
      console.log('server action', action);

      if (!action.type) {
        throw new Error('Empty action.type: you likely mistyped the action.');
      }


      AppDispatcher.dispatch({
        source: AppConstants.SERVER_ACTION,
        action: action
      });

    },

    handleViewAction: function(action) {
      console.log('view action', action);

      if (!action.type) {
        throw new Error('Empty action.type: you likely mistyped the action.');
      }


      AppDispatcher.dispatch({
        source: AppConstants.VIEW_ACTION,
        action: action
      });

    }
  });

  module.exports = AppDispatcher;
}());
