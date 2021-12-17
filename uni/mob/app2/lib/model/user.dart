import 'dart:convert';

import 'package:http/http.dart' as http;

class User {
  String first, last;
  String email;
  String avatar;

  User(this.first, this.last, this.email, this.avatar);

  User.fromJson(Map<String, dynamic> json)
      : first = json["name"]["first"],
        last = json["name"]["last"],
        email = json["email"],
        avatar = json["picture"]["large"];

  get fullName => "$first $last";
}

Future<List<User>> loadUsers() async {
  final url = Uri.parse("https://randomuser.me/api/?results=500");
  final response = await http.get(url);
  final jsonData = jsonDecode(response.body);
  final List results = jsonData["results"];
  return results.map((userJson) => User.fromJson(userJson)).toList();
}
