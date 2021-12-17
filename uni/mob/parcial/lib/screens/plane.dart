import 'package:flutter/material.dart';
import 'package:parcial/models/asientos.dart';
import 'package:parcial/models/pasajero.dart';

class PlaneSeats extends StatefulWidget {
  const PlaneSeats({Key? key}) : super(key: key);

  @override
  _PlaneSeatsState createState() => _PlaneSeatsState();
}

class _PlaneSeatsState extends State<PlaneSeats> {
  void checkSeat(int index) {
    bool emptySeat = true;
    for (var pasajero in pasajeros!) {
      if (pasajero.asiento == asientos[index]) {
        emptySeat = false;
        break;
      }
    }
    if (emptySeat) {
      Navigator.of(context).pop(asientos[index]);
    } else {
      showDialog<bool>(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: const Text("Asiento ocupado!"),
            content: Text(asientos[index]),
            actions: [
              TextButton(
                child: const Text("OK"),
                onPressed: () => Navigator.of(context).pop(),
              ),
            ],
          );
        },
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Elige asiento..."),
      ),
      body: Padding(
        padding: const EdgeInsets.all(20),
        child: GridView.builder(
          gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
            crossAxisCount: 7,
            childAspectRatio: 4 / 3,
            crossAxisSpacing: 6,
            mainAxisSpacing: 12,
          ),
          itemBuilder: (context, _index) {
            final col = (_index % 7);
            if (col == 3) {
              return Container();
            }
            final fila = (_index ~/ 7);
            final index = fila * 6 + (col < 3 ? col : col - 1);
            return GestureDetector(
              onTap: () {
                setState(() {
                  checkSeat(index);
                });
              },
              child: Container(
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(40),
                  border: Border.all(
                    color: Colors.black,
                    width: 3,
                  ),
                  color: Colors.teal,
                ),
                child: Center(
                  child: Text(
                    asientos[index],
                    style: const TextStyle(
                      fontSize: 32,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                ),
              ),
            );
          },
          itemCount: numLetras * numFilas,
        ),
      ),
    );
  }
}
