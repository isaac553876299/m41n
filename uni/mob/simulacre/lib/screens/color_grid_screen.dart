import 'package:flutter/material.dart';
import 'package:simulacre/colors.dart';

class ColorGridScreen extends StatefulWidget {
  const ColorGridScreen({Key? key}) : super(key: key);

  @override
  _ColorGridScreenState createState() => _ColorGridScreenState();
}

class _ColorGridScreenState extends State<ColorGridScreen> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Choose a Color..."),
      ),
      body: GridView.builder(
        gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 5,
          childAspectRatio: 1 / 1,
          crossAxisSpacing: 0,
          mainAxisSpacing: 0,
        ),
        itemBuilder: (context, index) {
          return GestureDetector(
            onTap: () {
              setState(() {
                //debugPrint("$index");
                Navigator.of(context).pop(colorList[index]);
              });
            },
            child: Container(
              color: colorList[index],
            ),
          );
        },
        itemCount: 190,
      ),
    );
  }
}
