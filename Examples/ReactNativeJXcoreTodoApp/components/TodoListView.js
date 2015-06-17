'use strict';

var React = require('react-native');
var {AppRegistry, StyleSheet, Text, View, ListView, Image, SwitchIOS} = React;

var TodoListItem = require('./TodoListItem.js');


var TodoListView = React.createClass({
  componentWillMount: function() {
    this.dataSource = new ListView.DataSource({
            rowHasChanged: (row1, row2) => row1 !== row2
        });
  },
  renderRow: function(rowData, sectionID, rowID, highlightRow) {
    return (<TodoListItem rowData={rowData} onItemChanged={this.props.onItemChanged} isEditMode={this.props.isEditMode} onDeleteItem={this.props.onDeleteItem}/>);
  },

  render: function() {
    var dataSource = this.dataSource.cloneWithRows(this.props.items);
    return (
      <ListView
      automaticallyAdjustContentInsets={false} dataSource={dataSource} renderRow={this.renderRow} />
      );
  }
});


module.exports = TodoListView;