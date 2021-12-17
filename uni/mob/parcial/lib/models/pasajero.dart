class Pasajero {
  String name;
  String asiento = "no asignado";
  Pasajero(this.name, this.asiento);
  Pasajero.nombrar(this.name);
  void setAsiento(String seat) => asiento = seat;
}

final List<Pasajero>? pasajeros = [
  Pasajero.nombrar("Obama"),
  Pasajero.nombrar("Bob el manetes"),
  Pasajero.nombrar("Doraemon"),
  Pasajero.nombrar("MazingerZ"),
  Pasajero.nombrar("Zeus"),
  Pasajero.nombrar("Cthulhu"),
  Pasajero.nombrar("H.P.Lovecraft"),
];
