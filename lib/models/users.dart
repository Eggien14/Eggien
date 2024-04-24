import 'package:firewise_app/constants/image_string.dart';

class Users {
  final String username;
  final String phone;
  final String address;
  final String password;
  final String userID;
  final String image;
  final String deviceID;

  // Constructor cho việc đăng nhập thành công
  Users({
    required this.userID,
    required this.username,
    required this.password,
    required this.phone,
    required this.address,
    required this.image,
    required this.deviceID,
  });
  // Constructor for SharedPreferences
  Users.sharedPreferences({
    required this.userID,
    required this.username,
    required this.phone,
    required this.address,
    required this.image,
    required this.deviceID,
  }) : password = '';
  // Constructor cho việc nhân thông tin đăng ký
  Users.register({
    required this.username,
    required this.password,
    required this.phone,
    required this.address,
  })  : userID = '',
        deviceID = '',
        image = IMAGE_DEFAULT; // Mặc định userID là rỗng khi đăng ký thành công
  factory Users.fromJson(Map<String, dynamic> json) {
    return Users(
      userID: json['userID'],
      username: json['username'],
      password: json['password'],
      phone: json['phone'],
      address: json['address'],
      image: json['image'],
      deviceID: json['deviceID'],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'userID': userID,
      'username': username,
      'password': password,
      'phone': phone,
      'address': address,
      'image': image,
      'deviceId': deviceID,
    };
  }

  // Phương thức set cho username
  Users updateUser(String newUsername, String address, String image) {
    return Users(
        userID: this.userID,
        username: newUsername,
        password: this.password,
        phone: this.phone,
        address: address,
        image: image,
        deviceID: this.deviceID);
  }
}
