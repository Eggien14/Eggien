import 'dart:async';

import 'package:firebase_database/firebase_database.dart';
import 'package:firewise_app/models/users.dart';
import 'package:firewise_app/utils/data_user.dart';

class DataFirebase {
  // get string data to
  Future<String> getData(String virtualPin) async {
    try {
      Users user = await FetchUserData.getDataUser();
      String uID = user.userID;
      DatabaseReference userRef = FirebaseDatabase.instance
          .ref()
          .child('Virtual Pins')
          .child(uID)
          .child(virtualPin);
      DataSnapshot snapshot = await userRef.get();
      return snapshot.value as String;
    } catch (e) {
      print(e.toString());
      return "";
    }
  }

  Future<bool> setData(String virtualPin, String data) async {
    try {
      Users user = await FetchUserData.getDataUser();
      String uID = user.userID;
      DatabaseReference userRef =
          FirebaseDatabase.instance.ref().child('Virtual Pins').child(uID);
      await userRef.update({virtualPin: data});
      return true;
    } catch (e) {
      print(e.toString());
      return false;
    }
  }

  Future<Stream<String>> getStream(String virtualPin) async {
    StreamController<String> controller = StreamController<String>();
    try {
      Users user = await FetchUserData.getDataUser();
      String uID = user.userID;
      DatabaseReference userRef = FirebaseDatabase.instance
          .ref()
          .child('Virtual Pins')
          .child(uID)
          .child(virtualPin);
      userRef.onValue.listen((event) {
        if (event.snapshot.value != null) {
          controller.add(event.snapshot.value as String);
        }
      });
    } catch (e) {
      print(e.toString());
    }
    return controller.stream;
  }
}
