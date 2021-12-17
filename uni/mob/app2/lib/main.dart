import 'package:flutter/material.dart';
import 'package:app2/model/user.dart';

void main() {
  runApp(const UserListScreen());
}

class UserListScreen extends StatefulWidget {
  const UserListScreen({Key? key}) : super(key: key);

  @override
  _UserListScreenState createState() => _UserListScreenState();
}

class _UserListScreenState extends State<UserListScreen> {
  List<User>? users;

  @override
  void initState() {
    super.initState();
    loadUsers().then((loadedUsers) {
      setState(() {
        users = loadedUsers;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text("User List")),
      body: users == null
          ? const Center(child: CircularProgressIndicator())
          : ListView.separated(
              itemCount: users!.length,
              itemBuilder: (context, index) {
                return ListTile(
                  leading: CircleAvatar(
                    backgroundImage: NetworkImage(
                      users![index].avatar,
                    ),
                  ),
                  title: Text(users![index].fullName),
                  subtitle: Text(users![index].email),
                );
              },
              separatorBuilder: (context, index) {
                return const Divider(
                  height: 1,
                  indent: 67,
                );
              }),
    );
  }
}
