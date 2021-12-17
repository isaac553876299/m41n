import 'package:flutter/material.dart';

class SmallGridDemoScreen extends StatelessWidget {
  const SmallGridDemoScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    List<int> numbers = [for (int i = 1; i <= 50; i++) i];
    return Scaffold(
      appBar: AppBar(title: Text("Small Grid Demo")),
      body: GridView.count(
        crossAxisCount: 4,
        children: numbers
            .map(
              (n) => Card(
                child: Center(
                  child: Text(
                    "$n",
                    style: TextStyle(fontSize: 30),
                  ),
                ),
              ),
            )
            .toList(),
      ),
    );
  }
}
