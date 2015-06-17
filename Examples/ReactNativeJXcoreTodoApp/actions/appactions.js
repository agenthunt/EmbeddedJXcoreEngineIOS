var AppDispatcher = require('../dispatcher/appdispatcher.js');
var AppConstants = require('../constants/appconstants.js');

var AppActions = {


  getItems: function() {
    AppDispatcher.handleViewAction({
      type: AppConstants.GET_ITEMS
    });
  },
  receiveItems: function(obj) {
    AppDispatcher.handleServerAction({
      type: AppConstants.RECEIVE_ITEMS,
      obj: obj
    });
  },
  addItem: function(obj) {
    AppDispatcher.handleViewAction({
      type: AppConstants.ADD_ITEM,
      obj: obj
    });
  },
  updateItem: function(obj) {
    AppDispatcher.handleViewAction({
      type: AppConstants.UPDATE_ITEM,
      obj: obj
    });
  },
  deleteItem: function(obj) {
    AppDispatcher.handleViewAction({
      type: AppConstants.DELETE_ITEM,
      obj: obj
    });
  }
};
module.exports = AppActions;
