import 'package:flutter/material.dart';
import 'package:flutter_webview_plugin/flutter_webview_plugin.dart';

class NewsScreen extends StatefulWidget {
  @override
  _newScreenState createState() => _newScreenState();
}

class _newScreenState extends State<NewsScreen> {
  String selectedUrl = 'https://www.google.com/';

// ignore: prefer_collection_literals
  final Set<JavascriptChannel> jsChannels = [
    JavascriptChannel(
        name: 'Print',
        onMessageReceived: (JavascriptMessage message) {
          print(message.message);
        })
  ].toSet();

  @override
  Widget build(BuildContext context) {
    // TODO: implement build
    return Scaffold(
        body: WebviewScaffold(
      url: selectedUrl,
      javascriptChannels: jsChannels,
      mediaPlaybackRequiresUserGesture: false,
      appBar: AppBar(
        title: const Text('Widget WebView'),
      ),
      withZoom: true,
      withLocalStorage: true,
      hidden: true,
      initialChild: Container(
        color: Colors.redAccent,
        child: const Center(
          child: Text('Waiting.....'),
        ),
      ),
    ));
  }
}
