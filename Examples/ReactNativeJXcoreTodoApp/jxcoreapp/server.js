// server.js

// BASE SETUP
// =============================================================================

// call the packages we need
var express = require('express'); // call express
var app = express(); // define our app using express

console.log("dirname:" + __dirname);
var Todos = require('./app/model/Todos.js');

var bodyParser = require('body-parser');

// configure app to use bodyParser()
// this will let us get the data from a POST
app.use(bodyParser.urlencoded({
  extended: true
}));
app.use(bodyParser.json());

var port = process.env.PORT || 8080; // set our port

// ROUTES FOR OUR API
// =============================================================================
var router = express.Router(); // get an instance of the express Router

// middleware to use for all requests
router.use(function(req, res, next) {
  // do logging
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next(); // make sure we go to the next routes and don't stop here
});

// test route to make sure everything is working (accessed at GET http://localhost:8080/api)
router.get('/', function(req, res) {
  res.json({
    message: 'A todos app sample with react native front end and embedded jxcore backend rest api (express, nedb)'
  });
});

// more routes for our API will happen here
// on routes that end in /items
// ----------------------------------------------------
router.route('/items')
  // create an item (accessed at POST http://localhost:8080/api/items)
  .post(function(req, res) {
    console.log(req.body.text);
    var item = {
      text: req.body.text,
      completed: false
    }; // set the items name (comes from the request)

    Todos.addItem(item, function(err, items) {
      if(err) {
        res.send(err);
      } else {
        res.json(items);
      }
    });
  })

.get(function(req, res) {
  Todos.getItems(function(err, items) {
    if(err) {
      res.send(err);
    } else {
      res.json(items);
    }
  });
});

//on routes that end in /items/:item_id
//----------------------------------------------------
router.route('/items/:item_id')

// update the item with this id (accessed at PUT http://localhost:8080/api/items/:item_id)
.put(function(req, res) {

    console.log(req.body);
    
    Todos.updateItem(req.body, function(err, items) {
      if(err) {
        res.send(err);
      } else {
        res.json(items);
      }
    });
  }) // delete the item with this id (accessed at DELETE http://localhost:8080/api/items/:item_id)
  .delete(function(req, res) {
    Todos.deleteItem(req.body, function(err, items) {
      if(err) {
        res.send(err);
      } else {
        res.json(items);
      }
    });
  });


// REGISTER OUR ROUTES -------------------------------
// all of our routes will be prefixed with /api
app.use('/api', router);

// START THE SERVER
// =============================================================================
app.listen(port);
