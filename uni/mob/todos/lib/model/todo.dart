class Todo {
  String what;
  bool done;
  Todo(this.what, [this.done = false]);

  void toggleDone() => done = !done;
}
