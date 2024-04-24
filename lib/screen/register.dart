// ignore_for_file: library_private_types_in_public_api, camel_case_types

import 'package:flutter/material.dart';
import 'package:firewise_app/models/users.dart';
import 'package:firewise_app/screen/login.dart';
import 'package:firewise_app/services/auth_firebase.dart';
import 'package:firewise_app/widgets/Button/button_form.dart';
import 'package:firewise_app/widgets/Button/button_social.dart';
import 'package:firewise_app/widgets/Text/text_button.dart';
import 'package:firewise_app/widgets/Text/text_field.dart';
import 'package:firewise_app/widgets/Text/text_title.dart';
import 'package:the_country_number_widgets/the_country_number_widgets.dart';

class RegisterScreen extends StatefulWidget {
  const RegisterScreen({super.key});

  @override
  _registerScreenState createState() => _registerScreenState();
}

class _registerScreenState extends State<RegisterScreen> {
  late TextEditingController _userNameEditingController;
  late String _phone;
  late TextEditingController _addressNameEditingController;
  late TextEditingController _passwordNameEditingController;

  @override
  void initState() {
    super.initState();
    _userNameEditingController = TextEditingController();
    _phone = "";
    _addressNameEditingController = TextEditingController();
    _passwordNameEditingController = TextEditingController();
  }

  @override
  void dispose() {
    // Clean up the controllers when the widget is disposed
    _userNameEditingController.dispose();
    _addressNameEditingController.dispose();
    _passwordNameEditingController.dispose();
    super.dispose();
  }

  final AuthService _auth = AuthService();
  @override
  Widget build(BuildContext context) {
    final _formKey = GlobalKey<FormState>();

    return Scaffold(
        resizeToAvoidBottomInset: true,
        backgroundColor: const Color.fromRGBO(247, 248, 250, 1),
        body: SingleChildScrollView(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              const SizedBox(
                height: 100,
              ),
              const SizedBox(
                child: TitleTextWidget(
                  text: "Create",
                  color: Colors.black,
                ),
              ),
              const SizedBox(
                child: TitleTextWidget(
                  text: "Your Account",
                  color: Colors.black,
                ),
              ),
              const SizedBox(
                height: 30,
              ),
              Padding(
                padding:
                    const EdgeInsets.symmetric(horizontal: 30, vertical: 40),
                child: Center(
                  child: Form(
                    key: _formKey,
                    child: Column(
                      children: [
                        TextFieldtWidget(
                          labelText: "User Name",
                          textEditingController: _userNameEditingController,
                          icon: Icons.email_outlined,
                          validator: (value) {
                            if (value == null || value.isEmpty) {
                              return 'Please enter user name';
                            }
                            return null;
                          },
                        ),
                        const SizedBox(
                          height: 10,
                        ),
                        TextFieldtWidget(
                          labelText: "Address",
                          textEditingController: _addressNameEditingController,
                          icon: Icons.account_balance_outlined,
                          validator: (value) {
                            if (value == null || value.isEmpty) {
                              return 'Please enter your address';
                            }
                            return null;
                          },
                        ),
                        const SizedBox(
                          height: 10,
                        ),
                        TheCountryNumberInput(
                          TheCountryNumber().parseNumber(iso2Code: "VN"),
                          onChanged: (tn) {
                            _phone = tn.number;
                          },
                          //custom validation
                          customValidator: (tn) {
                            final enteredNumber = tn.number;
                            //do something
                            String rs = validatePhoneNumber(enteredNumber);
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
                          textEditingController: _passwordNameEditingController,
                          icon: Icons.lock_clock_outlined,
                          validator: (value) {
                            if (value == null || value.isEmpty) {
                              return 'Please enter password';
                            }
                            return null;
                          },
                        ),
                        const SizedBox(
                          height: 20,
                        ),
                        ButtonFormWidget(
                          colorButton: const Color.fromARGB(255, 9, 11, 90),
                          colorText: Colors.white,
                          text: "Register",
                          onPressed: () {
                            // Validate form

                            if (_formKey.currentState!.validate()) {
                              // If form is valid, proceed with login action
                              _register();
                            }
                          },
                        ),
                        const SizedBox(
                          height: 10,
                        ),
                        const SizedBox(
                          height: 50,
                          child: Row(
                            mainAxisAlignment: MainAxisAlignment.center,
                            children: [
                              Text(
                                "Already Have An Account ?",
                                style: TextStyle(
                                    color: Color.fromARGB(255, 150, 147, 147)),
                              ),
                              TextButtonWidget(
                                  buttonText: "Sign in", screen: LoginScreen())
                            ],
                          ),
                        ),
                        const Divider(
                          color: Color.fromARGB(255, 150, 147, 147),
                          height: 50,
                        ),
                        SocialButtonRow(
                            onGooglePressed: () {}, onFacebookPressed: () {}),
                      ],
                    ),
                  ),
                ),
              )
            ],
          ),
        ));
  }

  Future<void> _register() async {
    try {
      String username = _userNameEditingController.text;
      String phone = _phone;
      String address = _addressNameEditingController.text;
      String password = _passwordNameEditingController.text;
      Users user = Users.register(
          username: username,
          password: password,
          phone: phone,
          address: address);
      // store in firebase
      if (await _auth.registerUser(user)) {
        print("Register successfully");
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Register Successfully')),
        );
        Navigator.pushReplacement(context,
            MaterialPageRoute(builder: (context) => const LoginScreen()));
      } else {
        print("Register Fail");
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Register Fail !')),
        );
      }
    } catch (e) {
      print(e.toString());
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
