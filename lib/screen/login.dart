// ignore_for_file: use_build_context_synchronously, library_private_types_in_public_api, camel_case_types

import 'package:flutter/material.dart';
import 'package:iot_app/Layout/layout.dart';
import 'package:iot_app/models/users.dart';
import 'package:iot_app/screen/register.dart';
import 'package:iot_app/services/auth_firebase.dart';
import 'package:iot_app/utils/data_user.dart';
import 'package:iot_app/utils/user_provider.dart';
import 'package:iot_app/widgets/Button/button_form.dart';
import 'package:iot_app/widgets/Button/button_social.dart';
import 'package:iot_app/widgets/Notice/notice_snackbar.dart';
import 'package:iot_app/widgets/Text/text_button.dart';
import 'package:iot_app/widgets/Text/text_field.dart';
import 'package:iot_app/widgets/Text/text_title.dart';
import 'package:provider/provider.dart';
import 'package:shared_preferences/shared_preferences.dart';

class LoginScreen extends StatefulWidget {
  const LoginScreen({super.key});

  @override
  _loginScreenState createState() => _loginScreenState();
}

class _loginScreenState extends State<LoginScreen> {
  late TextEditingController _emailEditingController;
  late TextEditingController _passwordEditingController;
  String? storedData;

  @override
  void initState() {
    super.initState();
    _emailEditingController = TextEditingController();
    _passwordEditingController = TextEditingController();
  }

  @override
  void dispose() {
    // Clean up the controllers when the widget is disposed
    _emailEditingController.dispose();
    _passwordEditingController.dispose();
    super.dispose();
  }

  final AuthService _auth = AuthService();
  @override
  Widget build(Object context) {
    final _formKey = GlobalKey<FormState>();
    return Scaffold(
        resizeToAvoidBottomInset: true,
        backgroundColor: Color.fromRGBO(247, 248, 250, 1),
        body: SingleChildScrollView(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              const SizedBox(
                height: 100,
              ),
              const SizedBox(
                width: 150,
                height: 50,
                child: TitleTextWidget(text: "Login"),
              ),
              const SizedBox(
                height: 100,
              ),
              Padding(
                padding:
                    const EdgeInsets.symmetric(horizontal: 30, vertical: 40),
                child: Center(
                  child: Column(
                    children: [
                      Form(
                          key: _formKey,
                          child: Column(
                            children: [
                              TextFieldtWidget(
                                labelText: "Email",
                                textEditingController: _emailEditingController,
                                icon: Icons.email_outlined,
                                validator: (value) {
                                  if (value == null || value.isEmpty) {
                                    return 'Please enter your email';
                                  }
                                  return null;
                                },
                              ),
                              const SizedBox(
                                height: 10,
                              ),
                              TextFieldtWidget(
                                labelText: "Password",
                                textEditingController:
                                    _passwordEditingController,
                                icon: Icons.lock_clock_outlined,
                                validator: (value) {
                                  if (value == null || value.isEmpty) {
                                    return 'Please enter your password';
                                  }
                                  return null;
                                },
                              ),
                            ],
                          )),
                      const SizedBox(
                          height: 50,
                          child: TextButtonWidget(
                              buttonText:
                                  "                              Forget Password ?",
                              screen: LoginScreen())),
                      const SizedBox(
                        height: 10,
                      ),
                      ButtonFormWidget(
                        colorButton: Color.fromARGB(255, 9, 11, 90),
                        colorText: Colors.white,
                        text: "Log in",
                        onPressed: () {
                          // Validate form
                          if (_formKey.currentState!.validate()) {
                            // If form is valid, proceed with login action
                            _login();
                          }
                        },
                      ),
                      const SizedBox(
                        height: 50,
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              "Create New Account ?",
                              style: TextStyle(
                                  color: Color.fromARGB(255, 150, 147, 147)),
                            ),
                            TextButtonWidget(
                                buttonText: "Sign up", screen: RegisterScreen())
                          ],
                        ),
                      ),
                      const Divider(
                        color: Color.fromARGB(255, 150, 147, 147),
                        height: 50,
                      ),
                      SocialButtonRow(
                          onGooglePressed: () async {
                            dynamic result = await _auth.signInAnon();
                            if (result == null) {
                              print("error sign in anonymous.");
                            } else {
                              print(result);
                            }
                          },
                          onFacebookPressed: () async {}),
                    ],
                  ),
                ),
              )
            ],
          ),
        ));
  }

  void _login() async {
    try {
      String email = _emailEditingController.text;
      String password = _passwordEditingController.text;
      Users user = await _auth.loginUser(email, password);
      // save in SharedPreferences
      SharedPreferences prefs = await SharedPreferences.getInstance();
      prefs.setString('userID', user.userID);
      prefs.setString('username', user.username); // Changed key to 'username'
      prefs.setString('email', user.email);
      prefs.setString('address', user.address);
      prefs.setString('image', user.image);
      // update user into provider
      Provider.of<UserProvider>(context, listen: false).updateUser(user);
      // Fetch stored data after saving
      if (await FetchUserData.setDataUser(user)) {
        print("Log in successfully");
        const CustomSnackBar(
            message: "Login successfully", seconds: 3); // message

        //go ahead
        Navigator.pop(context);
        Navigator.of(context)
            .pushReplacement(MaterialPageRoute(builder: (context) => Layout()));
      } else {
        throw Exception();
      }
    } catch (e) {
      print(e.toString());
      const CustomSnackBar(message: "Login fail, Try again !", seconds: 3);
    }
  }
}
