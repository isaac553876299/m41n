import 'package:flutter/material.dart';
import 'package:parcial/models/pasajero.dart';
import 'package:parcial/screens/plane.dart';

class ListaPasajeros extends StatefulWidget {
  const ListaPasajeros({Key? key}) : super(key: key);

  @override
  _ListaPasajerosState createState() => _ListaPasajerosState();
}

class _ListaPasajerosState extends State<ListaPasajeros> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Lista de Pasajeros")),
      body: pasajeros == null
          ? const Center(child: CircularProgressIndicator())
          : ListView.separated(
              itemCount: pasajeros!.length,
              itemBuilder: (context, index) {
                return ListTile(
                  leading: const CircleAvatar(
                    child: Icon(
                      Icons.person,
                      color: Colors.black,
                      size: 50,
                    ),
                  ),
                  title: Text(
                    pasajeros![index].name,
                    style: const TextStyle(
                      fontSize: 20,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  subtitle: Text(
                    "asiento: " + pasajeros![index].asiento.toString(),
                    style: TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.w600,
                      color: Colors.grey[600],
                    ),
                  ),
                  onTap: () {
                    Navigator.of(context)
                        .push(
                      MaterialPageRoute(
                        builder: (context) => const PlaneSeats(),
                      ),
                    )
                        .then((seat) {
                      setState(() {
                        pasajeros![index].setAsiento(seat);
                      });
                    });
                  },
                );
              },
              separatorBuilder: (context, index) {
                return const Divider(height: 1, indent: 67);
              },
            ),
    );
  }
}
