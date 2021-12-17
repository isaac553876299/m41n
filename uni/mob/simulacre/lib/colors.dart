import 'package:flutter/material.dart';

const swatchList = [
  Colors.red,
  Colors.pink,
  Colors.purple,
  Colors.deepPurple,
  Colors.indigo,
  Colors.blue,
  Colors.lightBlue,
  Colors.cyan,
  Colors.teal,
  Colors.green,
  Colors.lightGreen,
  Colors.lime,
  Colors.yellow,
  Colors.amber,
  Colors.orange,
  Colors.deepOrange,
  Colors.brown,
  Colors.grey,
  Colors.blueGrey,
];

final colorList = [
  for (var swatch in swatchList) ...[
    swatch[50],
    for (int i = 1; i <= 9; i++) swatch[100 * i],
  ]
];
