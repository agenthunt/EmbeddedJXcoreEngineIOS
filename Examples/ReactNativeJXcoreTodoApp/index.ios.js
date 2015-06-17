/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 */
'use strict';

var React = require('react-native');
var {AppRegistry, StyleSheet, Text, View, Navigator,TouchableOpacity } = React;
var TodoMainView = require('./components/TodoMainView.js');


var isEditMode = false;

var NavigationBarRouteMapper = {


  LeftButton: function(route, navigator, index, navState) {
    return null;
  },

  RightButton: function(route, navigator, index, navState) {
    return (
       <TouchableOpacity
        onPress={() => {
          isEditMode =!isEditMode;
          navigator.replace({
        name: 'TodoMainView',
        index: 0
      })}}>
        <View style={styles.navBarRightButton}>
          <Text style={[styles.navBarText, styles.navBarButtonText]}>
            {isEditMode === true ?"Done":"Edit"}
          </Text>
        </View>
      </TouchableOpacity>
      );
  },

  Title: function(route, navigator, index, navState) {
    return (
      <Text style={[styles.navBarText, styles.navBarTitleText]}>
        todos
      </Text>
      );
  },

};

var ReactNativeJXcoreTodoApp = React.createClass({
  getInitialState: function() {
    return {
      isEditMode:false 
    };
  },
  render: function() {
    return (
      <Navigator style={styles.nav}
      navigationBar={
      <Navigator.NavigationBar
      routeMapper={NavigationBarRouteMapper}
      style={styles.navBar}/>}
      initialRoute={{
        name: 'TodoMainView',
        index: 0
      }}
      renderScene={(route, navigator) => <TodoMainView
        title="todos"
        isEditMode={isEditMode}/>
      }/>
      );
  }
});

var styles = StyleSheet.create({
  navBar: {
    backgroundColor: '#f5f5f5',
  },
  navBarText: {
    fontSize: 22,
    fontWeight: '100',
    marginVertical: 10,
    color: '#AF2F2F'
  },
  navBarTitleText: {
    fontWeight: '100',
    fontSize: 40,
    marginVertical: 9,
    color: '#AF2F2F',
    fontFamily: 'Helvetica Neue',
    opacity: 0.5
  },
  navBarLeftButton: {
    paddingLeft: 10,
  },
  navBarRightButton: {
    paddingRight: 10,
    paddingTop:10
  }
});

AppRegistry.registerComponent('ReactNativeJXcoreTodoApp', () => ReactNativeJXcoreTodoApp);