import 'package:flutter/material.dart';
import 'package:app3/screens/small_grid_demo_screen.dart';
import 'package:app3/screens/big_grid_demo_screen.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      home: BigGridDemoScreen(),
    );
  }
}
