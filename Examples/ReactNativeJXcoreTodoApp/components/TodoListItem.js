var React = require('react-native');
var {StyleSheet, Text, View, Image, SwitchIOS, TouchableOpacity, LayoutAnimation} = React;

var maxlimit = 40;
var CIRCLE_SIZE = 25;
var CIRCLE_COLOR = '#EF2F2F';


var DeleteButton = React.createClass({
  render: function() {
    return (
      <TouchableOpacity onPress={this.props.onPressed}>
             <Image
      source={require('image!delete')}/>
        </TouchableOpacity>
      );
  }
});


var TodoListItem = React.createClass({
  onSwitchValueChange: function(value) {
    var item = Object.assign({}, this.props.rowData);
    item.completed = value;
    this.props.onItemChanged(item);
  },
  getTrimmedText: function(rowData) {
    return ((rowData.text).length > maxlimit) ?
      (((rowData.text).substring(0, maxlimit - 3)) + '...') :
      rowData.text
  },
  onDeleteButtonPressed: function() {
    this.props.onDeleteItem(this.props.rowData);
  },
  showHideDeleteButton: function(value) {
    if (value) {
      return (
        <DeleteButton onPressed={this.onDeleteButtonPressed}/>
        );
    }
    return null;
  },
  render: function() {
    var rowData = this.props.rowData;
    return (
      <View style={styles.container}>
      <View style={styles.row}>
        {this.showHideDeleteButton(this.props.isEditMode)}
        <View style={styles.alignAtEnds}>
          <Text style={styles.text}>
            {this.getTrimmedText(rowData)}
          </Text>
          <SwitchIOS onValueChange={this.onSwitchValueChange}
      style={styles.switchStyle}
      value={rowData.completed} />
          </View>
      </View>
      <View style={styles.divider}/>
      </View>
      );
  }

});

var styles = StyleSheet.create({
  row: {
    flex: 1,
    flexDirection: 'row',
    justifyContent: 'center',
    alignItems: 'center',
    padding: 5
  },
  divider: {
    backgroundColor: '#f0f0f0',
    height: 1,
    flex: 1,
    marginLeft: 0,
    marginRight: 0
  },
  text: {
    fontSize: 24,
    color: '#4d4d4d',
    fontFamily: 'Helvetica Neue',
    fontWeight: '200',
    flex: 3,
  },
  switchStyle: {
    marginRight: 0
  },
  alignAtEnds: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    flex: 1,
    margin: 10
  },
  circle: {
    backgroundColor: CIRCLE_COLOR,
    width: CIRCLE_SIZE,
    height: CIRCLE_SIZE,
    borderRadius: CIRCLE_SIZE / 2,
    marginLeft: 5
  },
  container: {
    flex: 1
  }
});




module.exports = TodoListItem;