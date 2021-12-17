import 'package:flutter/material.dart';
import 'package:todos/model/todo.dart';

class TodoListScreen extends StatefulWidget {
  const TodoListScreen({Key? key}) : super(key: key);

  @override
  _TodoListScreenState createState() => _TodoListScreenState();
}

class _TodoListScreenState extends State<TodoListScreen> {
  late TextEditingController controller;

  List<Todo> todos = [
    Todo("Comprar patatas"),
    Todo("Estudiar DM"),
    Todo("Tasca 1 Motores", true),
  ];

  @override
  void initState() {
    super.initState();
    controller = TextEditingController();
  }

  @override
  void dispose() {
    controller.dispose();
    super.dispose();
  }

  void confirmDelete(int index) {
    showDialog<bool>(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: const Text("Confirm Delete"),
            content: Text("r u sure u want to dilit '${todos[index].what}' ?"),
            actions: [
              TextButton(
                child: const Text("Cancel"),
                onPressed: () => Navigator.of(context).pop(false),
              ),
              TextButton(
                child: const Text("Ok"),
                onPressed: () => Navigator.of(context).pop(true),
              ),
            ],
          );
        }).then((result) {
      if (result != null && result) {
        setState(() {
          todos.removeAt(index);
        });
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("Todo List")),
      body: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Expanded(
            child: ListView.builder(
              itemCount: todos.length,
              itemBuilder: (context, index) {
                return ListTile(
                  onTap: () {
                    setState(() {
                      todos[index].toggleDone();
                    });
                  },
                  onLongPress: () {
                    setState(() {
                      confirmDelete(index);
                    });
                  },
                  leading: Checkbox(
                    value: todos[index].done,
                    onChanged: (newValue) {
                      setState(() {
                        todos[index].toggleDone();
                      });
                    },
                  ),
                  title: Text(
                    todos[index].what,
                    style: TextStyle(
                      decoration: (todos[index].done
                          ? TextDecoration.lineThrough
                          : TextDecoration.none),
                    ),
                  ),
                );
              },
            ),
          ),
          Container(
            height: 80,
            color: Colors.blue[100],
            child: Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: controller,
                    onSubmitted: (text) {
                      setState(() {
                        if (text.isNotEmpty) {
                          todos.add(Todo(text));
                          controller.clear();
                        }
                      });
                    },
                  ),
                ),
                IconButton(
                  icon: const Icon(Icons.add),
                  onPressed: () {
                    setState(() {
                      if (controller.text.isNotEmpty) {
                        todos.add(Todo(controller.text));
                        controller.clear();
                      }
                    });
                  },
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
