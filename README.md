
# Firewise App

## Introduction

`firewise_app` is a fire alert software designed for home use. Developed using Flutter, it aims to alert users about fire hazards within their homes.

## Getting Started

### Requirements

Before getting started, ensure you have the following installed:

- [Flutter SDK](https://docs.flutter.dev/get-started/install)
- [Android Studio](https://developer.android.com/studio)
- [Visual Studio Code](https://code.visualstudio.com/)
- For demo purposes: Use either the Android Studio emulator or connect a physical device via USB debugging with Developer permissions enabled.

### Installation

1. **Clone the Repository**

   ```bash
   git clone https://github.com/Eggien14/FireWiseTech.git
   ```

2. **Download google-services.json from Firebase**
   - Access [Firebase Console](https://console.firebase.google.com/project/fire-wise-tech/settings/general/android:com.firewise.firewise_app) using the account registered to join the **firewise tech** project.
   - In Firebase console -> Project Setting -> Your App, download the file `google-services.json`.
   - Once downloaded, move `google-services.json` into the `android/app` folder of this project.

3. **Navigate to the project directory**

   ```bash
   cd firewise_app
   ```

4. **Install dependencies**

   ```bash
   flutter pub get
   ```

5. **Run the app on an emulator or physical device**

   ```bash
   flutter run
   ```

### Build for Android:

1. Generate APK file:
   ```bash
   flutter build apk
   ```
   This command will generate an APK file in the `build/app/outputs/apk/release` directory, suitable for installation on Android devices.

2. Generate App Bundle file:
   ```bash
   flutter build appbundle
   ```
   This command will generate an App Bundle (`.aab`) file in the `build/app/outputs/bundle/release` directory. The App Bundle is the recommended format for uploading the app to the Google Play Store.

### Build for iOS:

1. Generate iOS build:
   ```bash
   flutter build ios
   ```
   This command will generate a `build/ios` directory containing all necessary files for deploying the app to an iOS device or packaging it for submission to the App Store.

2. Run release version on iOS device:
   ```bash
   flutter run --release
   ```

## Screenshots

Here are some screenshots illustrating the app:

### Welcome Screen
![Welcome Screen](https://i.ibb.co/t8RJCsR/welcome-screen.png)

### Login Screen
![Login Screen](https://i.ibb.co/MRCLtc3/Login-screen.png)

### Register Screen
![Signin Screen](https://i.ibb.co/wMNRjpx/Signin-Screen.png)
```

