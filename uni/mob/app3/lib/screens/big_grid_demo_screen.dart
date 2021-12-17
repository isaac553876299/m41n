import 'package:flutter/material.dart';

class BigGridDemoScreen extends StatefulWidget {
  const BigGridDemoScreen({Key? key}) : super(key: key);

  @override
  _BigGridDemoScreenState createState() => _BigGridDemoScreenState();
}

class _BigGridDemoScreenState extends State<BigGridDemoScreen> {
  Set<int> selected = {1, 7, 15, 23, 100, 1000};

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Big Grid Demo"),
      ),
      body: GridView.builder(
        gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: 7,
            childAspectRatio: 5 / 4,
            crossAxisSpacing: 5,
            mainAxisSpacing: 5),
        itemBuilder: (context, index) {
          return GestureDetector(
            onTap: () {
              setState(() {
                if (selected.contains(index)) {
                  selected.remove(index);
                } else {
                  selected.add(index);
                }
              });
            },
            child: Container(
              color:
                  (selected.contains(index) ? Colors.amber : Colors.blue[100]),
              child: Center(
                child: Text(
                  "$index",
                  style: const TextStyle(fontSize: 20),
                ),
              ),
            ),
          );
        },
      ),
    );
  }
}
