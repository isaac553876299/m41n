import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: Container(
            padding: const EdgeInsets.fromLTRB(20, 20, 20, 20),
            child: Column(
              mainAxisSize: MainAxisSize.min,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Expanded(
                  flex: 1,
                  child: Container(
                    alignment: Alignment.centerLeft,
                    child: const Icon(
                      Icons.arrow_back,
                      size: 40,
                      color: Colors.black,
                    ),
                  ),
                ),
                Expanded(
                  flex: 2,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    crossAxisAlignment: CrossAxisAlignment.center,
                    children: [
                      const Expanded(
                        flex: 4,
                        child: Text(
                          "The Tretes Kopian Janji Jiwa",
                          style: TextStyle(
                            fontSize: 30,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),
                      Expanded(
                        flex: 2,
                        child: Container(
                          alignment: Alignment.center,
                          child: FloatingActionButton(
                            onPressed: () {},
                            backgroundColor: Colors.amber[800],
                            foregroundColor: Colors.white,
                            child: const Icon(
                              Icons.bookmark,
                              size: 25,
                            ),
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
                Expanded(
                  flex: 5,
                  child: Stack(
                    children: [
                      Image.asset(
                        "assets/image.png",
                        fit: BoxFit.fitHeight,
                        scale: 0.5,
                      ),
                      Container(
                        padding: const EdgeInsets.only(top: 225, left: 425),
                        child: FloatingActionButton(
                          onPressed: () {},
                          backgroundColor: Colors.white,
                          foregroundColor: Colors.black,
                          child: const Icon(
                            Icons.zoom_out_map,
                            size: 25,
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
                Expanded(
                  flex: 2,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    children: const [
                      Button1(iconData: Icons.room_service, text: "Food"),
                      Button1(iconData: Icons.local_parking, text: "Parking"),
                      Button1(iconData: Icons.wifi, text: "Wifi"),
                      Button1(iconData: Icons.coffee, text: "Coffee"),
                    ],
                  ),
                ),
                Expanded(
                  flex: 1,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Container(
                        alignment: Alignment.centerLeft,
                        child: const Text(
                          "Popular Times",
                          style: TextStyle(
                            fontSize: 20,
                            fontWeight: FontWeight.bold,
                          ),
                        ),
                      ),
                      Container(
                        alignment: Alignment.centerRight,
                        width: 100,
                        height: 50,
                        decoration: BoxDecoration(
                          borderRadius:
                              const BorderRadius.all(Radius.circular(15)),
                          color: Colors.white,
                          border: Border.all(color: Colors.grey),
                        ),
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                          children: const [
                            Text(
                              "Today",
                              style: TextStyle(
                                fontSize: 16,
                                fontWeight: FontWeight.w600,
                              ),
                            ),
                            Icon(
                              Icons.arrow_drop_down,
                              size: 25,
                              color: Colors.black,
                            ),
                          ],
                        ),
                      ),
                    ],
                  ),
                ),
                Expanded(
                  flex: 4,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      const Bar(text: "9am", value: 8),
                      const Bar(text: "10am", value: 6),
                      const Bar(text: "11am", value: 8),
                      Bar(text: "12am", value: 10, color: Colors.amber[800]),
                      const Bar(text: "1pm", value: 7),
                      const Bar(text: "2pm", value: 5),
                      const Bar(text: "3pm", value: 7),
                      const Bar(text: "4pm", value: 9),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class Button1 extends StatelessWidget {
  final IconData? iconData;
  final String? text;
  const Button1({
    Key? key,
    this.iconData = Icons.error,
    this.text = "",
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 75,
      child: Column(
        children: [
          FloatingActionButton(
            onPressed: () {},
            backgroundColor: Colors.teal,
            foregroundColor: Colors.white,
            child: Icon(iconData, size: 25),
          ),
          Text(
            text!,
            style: const TextStyle(
              fontSize: 12,
              fontWeight: FontWeight.w600,
              color: Colors.grey,
            ),
          ),
        ],
      ),
    );
  }
}

class Bar extends StatelessWidget {
  final String text;
  final double value;
  final Color? color;
  const Bar({
    Key? key,
    this.text = "",
    this.value = 10,
    this.color = Colors.black,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Column(
      mainAxisAlignment: MainAxisAlignment.end,
      children: [
        Container(
          margin: const EdgeInsets.symmetric(horizontal: 15),
          alignment: Alignment.topCenter,
          width: 12,
          height: value * 20,
          decoration: BoxDecoration(
            color: color,
            borderRadius: BorderRadius.circular(10),
          ),
        ),
        Container(
          alignment: Alignment.bottomCenter,
          child: Text(
            text,
            style: const TextStyle(
              fontSize: 15,
              fontWeight: FontWeight.w500,
              color: Colors.grey,
            ),
          ),
        ),
      ],
    );
  }
}
