import 'dart:io';

import 'package:flutter/material.dart';
import 'package:firewise_app/Layout/layout.dart';
import 'package:firewise_app/models/users.dart';
import 'package:firewise_app/services/auth_firebase.dart';
import 'package:firewise_app/utils/data_user.dart';
import 'package:firewise_app/utils/image_picker.dart';

class ProfileSetting extends StatefulWidget {
  const ProfileSetting({Key? key}) : super(key: key);

  @override
  _ProfileSettingState createState() => _ProfileSettingState();
}

class _ProfileSettingState extends State<ProfileSetting> {
  TextEditingController _usernameController = TextEditingController();
  TextEditingController _addressController = TextEditingController();

  late Users user;
  late String _image;
  bool isDataLoaded = false;

  @override
  void initState() {
    super.initState();
    FetchUserData1();
  }

  Future<void> FetchUserData1() async {
    try {
      user = await FetchUserData.getDataUser();
      _image = user.image;
      // Gán giá trị cũ vào các TextEditingController
      _usernameController.text = user.username;
      _addressController.text = user.address;
      setState(() {
        isDataLoaded = true;
      });
    } catch (e) {
      print(e.toString());
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Edit Profile'),
        // Nút "Back" trên App bar
        leading: IconButton(
          icon: Icon(Icons.arrow_back),
          onPressed: () {
            Navigator.pushReplacement(
                context, MaterialPageRoute(builder: (context) => Layout()));
          },
        ),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: EdgeInsets.all(20),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Center(
                child: Stack(
                  children: [
                    CircleAvatar(
                      radius: 60,
                      backgroundImage:
                          _image.isNotEmpty ? FileImage(File(_image)) : null,
                    ),
                    Positioned(
                      bottom: 0,
                      right: 0,
                      child: GestureDetector(
                        onTap: () {
                          _imagePicker();
                        },
                        child: Container(
                          padding: EdgeInsets.all(4),
                          decoration: BoxDecoration(
                            color: Colors.white,
                            shape: BoxShape.circle,
                          ),
                          child: Icon(
                            Icons.camera_alt,
                            color: Colors.grey,
                          ),
                        ),
                      ),
                    ),
                  ],
                ),
              ),
              SizedBox(height: 20),
              const Text(
                'User Name',
                style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
              ),
              TextField(
                controller: _usernameController,
              ),
              SizedBox(height: 20),
              const Text(
                'Address',
                style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
              ),
              TextField(
                controller: _addressController,
              ),
              const SizedBox(height: 20),
              ElevatedButton(
                onPressed: () {
                  _save();
                },
                child: const Text('Save Changes'),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Future<void> _save() async {
    // Viết mã để lưu thông tin mới
    String? _username = _usernameController.text;
    String? _address = _addressController.text;
    Users us = user.updateUser(_username, _address, _image);
    print(us.username + us.address);
    AuthService _auth = AuthService();
    if (await _auth.updateUserInfo(us)) {
      if (await FetchUserData.setDataUser(us)) {
        Navigator.pushReplacement(
            context, MaterialPageRoute(builder: (context) => Layout()));
      } else {
        print("FetchUserData.setDataUser fail");
      }
    } else {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Update user fail, Try again!')),
      );
    }
  }

  @override
  void dispose() {
    // Giải phóng bộ nhớ khi widget bị hủy
    _usernameController.dispose();
    _addressController.dispose();
    super.dispose();
  }

  void _imagePicker() async {
    final pickedFile = await pickImage();
    if (pickedFile != null) {
      setState(() {
        _image = pickedFile.path;
      });
    }
  }
}
