import 'package:firewise_app/models/users.dart';
import 'package:shared_preferences/shared_preferences.dart';

class FetchUserData {
  static Future<String?> getValue(String key) async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    return prefs.getString(key);
  }

  static Future<void> setValue(String key, String value) async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.setString(key, value);
  }

  static Future<Users> getDataUser() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    String? userId = prefs.getString('userID');
    String? userName = prefs.getString('userName');
    String? phone = prefs.getString('phone');
    String? address = prefs.getString('address');
    String? image = prefs.getString('image');
    String? deviceID = prefs.getString('deviceID');
    if (userId != null &&
        userName != null &&
        phone != null &&
        address != null &&
        image != null &&
        deviceID != null) {
      return Users.sharedPreferences(
        userID: userId,
        username: userName,
        phone: phone,
        address: address,
        image: image,
        deviceID: deviceID,
      );
    } else {
      throw Exception("Failed to fetch user data from SharedPreferences");
    }
  }

  static Future<bool> setDataUser(Users user) async {
    try {
      SharedPreferences prefs = await SharedPreferences.getInstance();
      prefs.setString('userID', user.userID);
      prefs.setString('userName', user.username);
      prefs.setString('deviceID', user.deviceID);
      prefs.setString('address', user.address);
      prefs.setString('image', user.image);
      prefs.setString('phone', user.phone);
      return true;
    } catch (e) {
      print(e.toString());
      return false;
    }
  }

  static Future<void> clearData() async {
    SharedPreferences prefs = await SharedPreferences.getInstance();
    await prefs.clear();
  }

  static Future<bool> clearDataUser() async {
    try {
      SharedPreferences prefs = await SharedPreferences.getInstance();
      prefs.setString('userID', "");
      prefs.setString('userName', "");
      prefs.setString('phone', "");
      prefs.setString('address', "");
      prefs.setString('image', "");
      prefs.setString('deviceID', "");

      return true;
    } catch (e) {
      print(e.toString());
      return false;
    }
  }
}
