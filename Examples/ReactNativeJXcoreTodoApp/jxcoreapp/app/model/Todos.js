// Type 2: Persistent datastore with manual loading
// 
console.log('HOME DIR:' + process.env['HOME']);
var Datastore = require('nedb'),
  db = new Datastore({
    filename: process.env['HOME'] + '/data/todos.db'
  });
db.loadDatabase(function(err) { // Callback is optional
  // Now commands will be executed
  console.log("loadingDatabase Error:" + err);
});

var Todos = {
  addItem: function(item, callback) {
  	console.log('inserting item' + JSON.stringify(item));
  	item._id = Date.now();
    db.insert(item, function(err) {
      if(err) {
        callback(err, null);
      } else {
        Todos.getItems(callback);
      }
    });
  },
  getItems: function(callback) {
    db.find({}).sort({_id:-1}).exec(function(err, items) {
      if(err) {
        callback(err, null);
      } else {
        callback(null, items)
      }
    });
  },
  updateItem: function(item, callback) {
    db.find(item, function(err, oldItem) {
      if(err) {
        callback(err, null);
      } else {
        db.update({
          _id: item._id
        }, item, function(err) {
          if(err) {
            callback(err, null);
          } else {
            Todos.getItems(callback);
          }
        });
      }
    })
  },
  deleteItem: function(item, callback) {
    console.log('deleting item' + item);
    db.remove( item, function(err, numRemoved) {
      console.log('numRemoved:' + numRemoved);
      if(err) {
        console.log(err);
        callback(err, null);
      } else {
        Todos.getItems(callback);
      }
    });
  }
};

module.exports = Todos;
