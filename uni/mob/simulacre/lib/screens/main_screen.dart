import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:simulacre/screens/color_grid_screen.dart';

class MainScreen extends StatefulWidget {
  const MainScreen({Key? key}) : super(key: key);

  @override
  _MainScreenState createState() => _MainScreenState();
}

class MyCustomScrollBehavior extends MaterialScrollBehavior {
  // Override behavior methods and getters like dragDevices
  @override
  Set<PointerDeviceKind> get dragDevices => {
        PointerDeviceKind.touch,
        PointerDeviceKind.mouse,
        // etc.
      };
}

class Saved {
  String text;
  bool bold;
  Color tColor;
  Color bColor;

  Saved(this.text, this.bold, this.tColor, this.bColor);
}

class _MainScreenState extends State<MainScreen> {
  late TextEditingController controller;
  String sampleText = "Sample Text";
  bool bold = true;
  Color bColor = Colors.yellow;
  Color tColor = Colors.black;

  List<Saved> saveds = [
    Saved("Sample Text", true, Colors.black, Colors.yellow),
    Saved("Sample Text", true, Colors.yellow, Colors.black),
  ];

  @override
  void initState() {
    super.initState();
    controller = TextEditingController(text: sampleText);
  }

  @override
  void dispose() {
    controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Color Match"),
      ),
      body: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Expanded(
            flex: 2,
            child: Container(
              color: bColor,
              child: Center(
                child: Text(
                  sampleText,
                  textAlign: TextAlign.center,
                  style: TextStyle(
                    color: tColor,
                    fontWeight: bold ? FontWeight.bold : FontWeight.normal,
                    fontSize: 32,
                  ),
                ),
              ),
            ),
          ),
          Expanded(
            flex: 4,
            child: Container(
              padding: const EdgeInsets.symmetric(horizontal: 20),
              child: Column(
                children: [
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Expanded(child: TextField(controller: controller)),
                      const SizedBox(width: 20),
                      OutlinedButton(
                        child: const Text("Set Text"),
                        onPressed: () {
                          setState(() {
                            sampleText = controller.text;
                          });
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 10),
                  CheckboxListTile(
                    title: const Text("Bold"),
                    value: bold,
                    onChanged: (newValue) {
                      setState(() {
                        bold = !bold;
                      });
                    },
                    controlAffinity: ListTileControlAffinity.leading,
                  ),
                  const SizedBox(height: 10),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Text(tColor.value.toRadixString(16)),
                      ElevatedButton(
                        child: const Text("Text"),
                        onPressed: () {
                          Navigator.of(context)
                              .push(
                            MaterialPageRoute(
                              builder: (context) => const ColorGridScreen(),
                            ),
                          )
                              .then((color) {
                            setState(() {
                              tColor = color;
                            });
                          });
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 10),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Text(bColor.value.toRadixString(16)),
                      TextButton(
                        child: const Text("Background"),
                        onPressed: () {
                          Navigator.of(context)
                              .push(
                            MaterialPageRoute(
                              builder: (context) => const ColorGridScreen(),
                            ),
                          )
                              .then((color) {
                            setState(() {
                              bColor = color;
                            });
                          });
                        },
                      ),
                    ],
                  ),
                  const SizedBox(height: 10),
                  ElevatedButton(
                    child: const Text("Save"),
                    onPressed: () {
                      setState(() {
                        saveds.add(Saved(sampleText, bold, tColor, bColor));
                      });
                    },
                  ),
                  const SizedBox(height: 10),
                  SizedBox(
                    height: 180.0,
                    child: ScrollConfiguration(
                      behavior: MyCustomScrollBehavior(),
                      child: ListView.builder(
                        shrinkWrap: true,
                        scrollDirection: Axis.horizontal,
                        itemCount: saveds.length,
                        itemBuilder: (BuildContext context, int index) =>
                            GestureDetector(
                          onTap: () {
                            setState(() {
                              sampleText = saveds[index].text;
                              bold = saveds[index].bold;
                              tColor = saveds[index].tColor;
                              bColor = saveds[index].bColor;
                              controller.text = saveds[index].text;
                            });
                          },
                          onSecondaryTap: () {
                            setState(() {
                              if (saveds.isNotEmpty) saveds.removeAt(index);
                            });
                          },
                          child: Container(
                            width: 125,
                            decoration: BoxDecoration(
                              borderRadius: BorderRadius.circular(10),
                              border: Border.all(
                                color: Colors.black,
                                width: 3,
                              ),
                            ),
                            child: Column(
                              children: [
                                Icon(
                                  Icons.text_snippet,
                                  color: saveds[index].tColor,
                                  size: 50,
                                ),
                                Icon(
                                  Icons.view_carousel,
                                  color: saveds[index].bColor,
                                  size: 50,
                                ),
                                Text(
                                  saveds[index].text,
                                  style: const TextStyle(
                                      fontSize: 16,
                                      fontWeight: FontWeight.bold),
                                ),
                                Icon(
                                  saveds[index].bold
                                      ? Icons.check_box_outlined
                                      : Icons.check_box_outline_blank,
                                  size: 50,
                                ),
                              ],
                            ),
                          ),
                        ),
                      ),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}
