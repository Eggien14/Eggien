// ignore_for_file: use_build_context_synchronously, library_private_types_in_public_api, camel_case_types

import 'package:flutter/material.dart';
import 'package:firewise_app/Layout/layout.dart';
import 'package:firewise_app/models/users.dart';
import 'package:firewise_app/screen/register.dart';
import 'package:firewise_app/services/auth_firebase.dart';
import 'package:firewise_app/utils/data_user.dart';
import 'package:firewise_app/utils/user_provider.dart';
import 'package:firewise_app/widgets/Button/button_form.dart';
import 'package:firewise_app/widgets/Button/button_social.dart';
import 'package:firewise_app/widgets/Text/text_button.dart';
import 'package:firewise_app/widgets/Text/text_field.dart';
import 'package:firewise_app/widgets/Text/text_title.dart';
import 'package:provider/provider.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:the_country_number_widgets/the_country_number_widgets.dart';

class LoginScreen extends StatefulWidget {
  const LoginScreen({super.key});

  @override
  _loginScreenState createState() => _loginScreenState();
}

class _loginScreenState extends State<LoginScreen> {
  late String _phone;
  late TextEditingController _passwordEditingController;
  String? storedData;
  @override
  void initState() {
    super.initState();

    _passwordEditingController = TextEditingController();
  }

  @override
  void dispose() {
    // Clean up the controllers when the widget is disposed
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
                child: TitleTextWidget(
                  text: "Login",
                  color: Colors.black,
                ),
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
                              TheCountryNumberInput(    
                                                            
                                TheCountryNumber().parseNumber(iso2Code: "VN"),
                                onChanged: (tn) {
                                  _phone = tn.number;
                                },
                                //custom validation
                                customValidator: (tn) {
                                  final enteredNumber = tn.number;
                                  //do something
                                  String rs =
                                      validatePhoneNumber(enteredNumber);
                                  if (rs != '') {
                                    return rs;
                                  }
                                  return null;
                                },
                                showDialCode: true,
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
      String phone = _phone;
      String password = _passwordEditingController.text;
      Users user = await _auth.loginUser(phone, password);
      // save user info in SharedPreferences
      SharedPreferences prefs = await SharedPreferences.getInstance();
      prefs.setString('userID', user.userID);
      prefs.setString('username', user.username); // Changed key to 'username'
      prefs.setString('phone', user.phone);
      prefs.setString('address', user.address);
      prefs.setString('image', user.image);
      prefs.setString('deviceID', user.deviceID);
      // update user into provider
      Provider.of<UserProvider>(context, listen: false).updateUser(user);
      // Fetch stored data after saving
      if (await FetchUserData.setDataUser(user)) {
        print("Log in successfully");
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Login Successfully')),
        );

        //go ahead
        Navigator.pop(context);
        Navigator.of(context)
            .pushReplacement(MaterialPageRoute(builder: (context) => Layout()));
      } else {
        throw Exception();
      }
    } catch (e) {
      print(e.toString());
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Login fail, Try again!')),
      );
    }
  }

  String validatePhoneNumber(String number) {
    final numbericRegex = RegExp(r'^[0-9]+$');
    if (number.isEmpty) return 'Please enter a phone number';
    if (!numbericRegex.hasMatch(number))
      return 'Please enter a valid phone number';
    return '';
  }
}
