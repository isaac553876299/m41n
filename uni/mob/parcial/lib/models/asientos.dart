const letras = ["A", "B", "C", "D", "E", "F"];

final numLetras = letras.length;
const numFilas = 7;
const numColumnas = 2;

// class Asiento {
//   String asiento;
//   bool libre;
//   Asiento(this.asiento, this.libre);
//   void setEmpty() => libre = !libre;
// }

final asientos = [
  for (int i = 1; i <= numFilas; i++) ...[
    for (var asiento in letras) i.toString() + asiento,
  ]
];

//no me da tiempo a mostrar asientos ocupados
//final _asientos = [for (var asiento in asientos) Asiento(asiento, false)];
