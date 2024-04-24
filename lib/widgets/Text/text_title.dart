import 'package:flutter/material.dart';

class TitleTextWidget extends StatelessWidget {
  final String text;
  final Color color;

  const TitleTextWidget({Key? key, required this.text, required this.color})
      : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Text(
      textAlign: TextAlign.center,
      text,
      style: TextStyle(color: color, fontSize: 30, fontWeight: FontWeight.bold),
    );
  }
}
