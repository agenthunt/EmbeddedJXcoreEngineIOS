'use strict';

var React = require('react-native');
var {AppRegistry, StyleSheet, Text, View, NavigatorIOS, TextInput, Navigator, TouchableHighlight, TouchableOpacity, ScrollView} = React;
var TodoListView = require('./TodoListView.js');
var AppStore = require('../stores/appstore.js');
var AppActions = require('../actions/appactions.js');


var TodoMainView = React.createClass({
  getInitialState: function() {
    return {
      input: "",
      items: []
    };
  },
  onAppStoreChange: function() {
    console.log('on appstore changed');
    if (this.isMounted()) {
      try {
        this.setState({
          items: AppStore.getItems(),
        });
      } catch (err) {
        console.error(err);
      }
    }
  },
  componentDidMount: function() {
    AppStore.addChangeListener(this.onAppStoreChange);
    this.processProps(this.props);

  },
  componentWillUnmount: function() {
    AppStore.removeChangeListener(this.onAppStoreChange);
  },
  componentWillReceiveProps: function(nextProps) {
    this.processProps(nextProps);
  },
  processProps: function(props) {
    AppActions.getItems();
  },
  onItemChanged: function(item) {
    AppActions.updateItem(item);
  },
  onDeleteItem: function(item) {
    AppActions.deleteItem(item);
  },
  render: function() {
    return (
      <View ref="ref" style={styles.container}>
      <Text style={styles.welcome}>
        A todos sample app with react native front end and embedded jxcore backend rest api (express, nedb)
      </Text>

      <View style={styles.paper}>
        <TextInput
      ref="input"
      style={styles.input}
      returnKeyType="done"
      placeholder="What needs to be done?"
      value={this.state.input}
      onSubmitEditing={(event) => {
        var text = event.nativeEvent.text;
        if (text.trim() != "") {
          AppActions.addItem({
            text: text
          });
          this.setState({
            input: ""
          });
          this.refs.input.focus();
        }
      }
      }
      onEndEditing={console.log('ending edit')}
      onChangeText={(text) => this.setState({
          input: text
        })}/>
       <View style={styles.divider}/> 
      <TodoListView style={styles.todoslist} items={this.state.items} onItemChanged={this.onItemChanged} isEditMode={this.props.isEditMode} onDeleteItem={this.onDeleteItem}/>
      </View>
      <View style={styles.paper2}>
      </View>
      <View style={styles.paper3}>
      </View>
      </View>
      );
  }
});


var styles = StyleSheet.create({
  container: {
    backgroundColor: '#f5f5f5',
    flex: 1,
    marginTop: 64
  },
  divider: {
    backgroundColor: '#f0f0f0',
    height: 1,
    marginLeft: 0,
    marginRight: 0
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
    color: '#4d4d4d',
    fontFamily: 'Helvetica Neue',
    fontWeight: '200'
  },
  input: {
    height: 40,
    borderColor: 'white',
    borderWidth: 1,
    fontFamily: 'Helvetica Neue',
    fontWeight: '100',
    padding: 6,
    margin: 10,
    color: '#4d4d4d',
    fontSize: 20,
    fontStyle: 'italic',
    shadowColor: "#000000",
    shadowOpacity: 0.3,
    shadowOffset: {
      height: 1,
      width: 0
    }
  },
  paper: {
    backgroundColor: 'white',
    marginLeft: 10,
    marginRight: 10,
    borderColor: '#cfcfcf',
    borderWidth: 1,
    flex: 1,
    shadowColor: "#000000",
    shadowOpacity: 0.3,
    shadowOffset: {
      height: 1,
      width: 0
    },
  },
  paper2: {
    backgroundColor: '#f5f5f5',
    marginLeft: 10,
    marginRight: 10,
    borderColor: '#cfcfcf',
    borderWidth: 1,
    height: 6,
    transform: [{
      scaleX: 0.98
    }]
  },
  paper3: {
    backgroundColor: '#f5f5f5',
    marginLeft: 10,
    marginRight: 10,
    marginBottom: 10,
    borderColor: '#cfcfcf',
    borderWidth: 1,
    shadowColor: "#000000",
    shadowOpacity: 0.3,
    shadowOffset: {
      height: 1,
      width: 0
    },
    height: 6,
    transform: [{
      scaleX: 0.96
    }]
  },
  todoslist: {
    height: 10,
    flex: 1
  }
});




module.exports = TodoMainView;