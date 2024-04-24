import 'package:flutter/material.dart';
import 'package:firewise_app/models/users.dart';
import 'package:firewise_app/utils/data_user.dart';
import 'package:firewise_app/utils/user_provider.dart';
import 'package:firewise_app/widgets/IoT/gauge.dart';
import 'package:firewise_app/widgets/IoT/gauge_mult.dart';
import 'package:firewise_app/widgets/IoT/switch.dart';
import 'package:provider/provider.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({super.key});

  @override
  // ignore: library_private_types_in_public_api
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  late Users? user;
  late Users storedData;
  bool isDataLoaded = false;
  late double x = 80;
  List<Widget> widgetsList = [];

  @override
  void initState() {
    super.initState();
    getUserData();
    FetchUserData1();
  }

  // ignore: non_constant_identifier_names
  Future<void> FetchUserData1() async {
    try {
      storedData = await FetchUserData.getDataUser();
      setState(() {
        isDataLoaded = true;
      });
    } catch (e) {
      print(e.toString());
    }
  }

  Future<void> getUserData() async {
    user = Provider.of<UserProvider>(context, listen: false).user;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Home"),
        actions: [
          IconButton(
            icon: Icon(Icons.add_box),
            onPressed: () {
              _addWidget();
            },
          ),
        ],
      ),
      body: SingleChildScrollView(
        child: Container(
            height: MediaQuery.of(context).size.height,
            padding: EdgeInsets.all(40),
            child: Column(
              children: [
                GridView.count(
                  physics: ClampingScrollPhysics(),
                  shrinkWrap: true,
                  crossAxisCount: 2, // Số cột
                  crossAxisSpacing: 10, // Khoảng cách giữa các cột
                  mainAxisSpacing: 10, // Khoảng cách giữa các hàng
                  padding: const EdgeInsets.only(bottom: 180),
                  children: [
                    IoTGaugeMulti(
                      virtualPin: "V1",
                      name: "Temperature",
                      max: 100,
                      min: 0,
                      meter: "*C",
                    ),
                    IoTGauge(
                      virtualPin: "V2",
                      name: "Huminity",
                      max: 100,
                      min: 0,
                      meter: "%",
                      color: Colors.blueAccent,
                    ),
                    Padding(
                      padding: EdgeInsets.all(10),
                      child: IoTSwitch(
                        virtualPin: "A1",
                        nameSwitch: "Light Yellow",
                        colorSwitch: Colors.yellow,
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.all(10),
                      child: IoTSwitch(
                        virtualPin: "A2",
                        nameSwitch: "Light Blue",
                        colorSwitch: Colors.blue,
                      ),
                    ),
                    Padding(
                      padding: EdgeInsets.all(10),
                      child: IoTSwitch(
                        virtualPin: "A3",
                        nameSwitch: "Light Green",
                        colorSwitch: Colors.green,
                      ),
                    ),
                    ...widgetsList,
                  ],
                ),
              ],
            )),
      ),
    );
  }

  void _addWidget() {
    // Thêm widget mới vào danh sách và cập nhật giao diện
    setState(() {
      widgetsList.add(
        const Padding(
          padding: EdgeInsets.all(10),
          child: IoTSwitch(
            virtualPin: "A1",
            nameSwitch: "Light Yellow",
            colorSwitch: Colors.yellow,
          ),
        ),
      );
    });
  }
}
