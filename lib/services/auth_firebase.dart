import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:firewise_app/models/users.dart';

class AuthService {
  final FirebaseAuth _auth = FirebaseAuth.instance;
  Future<User?> signInAnon() async {
    try {
      UserCredential result = await _auth.signInAnonymously();
      User? user = result.user;
      return user;
    } catch (e) {
      print(e.toString());
      return null;
    }
  }

  Future<Users> loginUser(String phone, String password) async {
    throw Error();
  }

  Future<bool> registerUser(Users user) async {
    throw Error();
  }

  Future<bool> updateUserInfo(Users user) async {
    return false;
  }
}
