/*
This is a UI file (.ui.qml) for the Karcha Tracker - Login Page
Matches the deep teal and warm amber theme exactly.
100% Declarative & Qt Design Studio Form Compliant (Fixed M225 Error).
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#F9FAFB"

    // Unified Color Palette (Mirrors Dashboard.ui.qml)
    readonly property color primaryColor: "#0F766E" // Deep Teal
    readonly property color accentColor: "#D97706"  // Warm Amber
    readonly property color cardBg: "#FFFFFF"
    readonly property color textDark: "#1F2937"
    readonly property color textMuted: "#6B7280"
    readonly property color borderColor: "#E5E7EB"

    // Property Aliases for Backend / Logic Layer Bindings
    property alias usernameText: usernameField.text
    property alias passwordText: passwordField.text
    property alias loginButton: loginBtn
    property alias forgotPasswordButton: forgotPasswordBtn
    property alias signUpButton: signUpBtn

    // Top subtle color stripe bar
    Rectangle {
        id: accentTopLine
        width: parent.width
        height: 6
        color: root.primaryColor
        anchors.top: parent.top
    }

    // Centered Login Card Module
    Rectangle {
        id: loginCard
        width: 450
        height: 580
        color: root.cardBg
        radius: 12
        border.color: root.borderColor
        border.width: 1
        anchors.centerIn: parent

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 40
            spacing: 24

            // App Identity Block
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6
                Layout.alignment: Qt.AlignHCenter

                Text {
                    text: "खर्च ट्र्याकर"
                    font.pixelSize: 32
                    font.bold: true
                    color: root.primaryColor
                    font.family: "Poppins"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Welcome back! Please sign in to your account."
                    font.pixelSize: 13
                    color: root.textMuted
                    font.family: "Inter"
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            // Input Form Group Section
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 16

                // Form Field 1: Identity Input
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Email or Username"
                        font.pixelSize: 12
                        font.bold: true
                        color: root.textMuted
                        font.family: "Inter"
                    }

                    TextField {
                        id: usernameField
                        Layout.fillWidth: true
                        height: 40
                        placeholderText: "Enter your username or email"
                        font.family: "Inter"
                        font.pixelSize: 13
                        color: root.textDark
                        leftPadding: 12

                        background: Rectangle {
                            id: usernameBg
                            color: "#F3F4F6"
                            radius: 8
                            border.color: usernameField.activeFocus ? root.primaryColor : root.borderColor
                            border.width: usernameField.activeFocus ? 2 : 1
                        }
                    }
                }

                // Form Field 2: Password Input
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Password"
                        font.pixelSize: 12
                        font.bold: true
                        color: root.textMuted
                        font.family: "Inter"
                    }

                    TextField {
                        id: passwordField
                        Layout.fillWidth: true
                        height: 40
                        placeholderText: "••••••••"
                        echoMode: TextInput.Password
                        font.family: "Inter"
                        font.pixelSize: 13
                        color: root.textDark
                        leftPadding: 12

                        background: Rectangle {
                            id: passwordBg
                            color: "#F3F4F6"
                            radius: 8
                            border.color: passwordField.activeFocus ? root.primaryColor : root.borderColor
                            border.width: passwordField.activeFocus ? 2 : 1
                        }
                    }
                }
            }

            // Secondary Utilities Line
            RowLayout {
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    id: forgotPasswordBtn
                    text: "Forgot Password?"
                    Layout.alignment: Qt.AlignRight
                    background: null

                    contentItem: Text {
                        text: parent.text
                        color: forgotPasswordBtn.hovered ? root.accentColor : root.primaryColor
                        font.pixelSize: 12
                        font.family: "Inter"
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }

            // Core Submission Button (Fixed using declarative ternary binding instead of internal states)
            Button {
                id: loginBtn
                Layout.fillWidth: true
                Layout.preferredHeight: 44
                text: "Sign In"
                font.pixelSize: 14
                font.bold: true
                font.family: "Inter"

                background: Rectangle {
                    id: loginBtnBg
                    // Declarative ternary syntax handles the hover change perfectly here
                    color: loginBtn.hovered ? "#0D5F5A" : root.primaryColor
                    radius: 8
                }

                contentItem: Text {
                    text: parent.text
                    color: "#FFFFFF"
                    font.pixelSize: parent.font.pixelSize
                    font.bold: parent.font.bold
                    font.family: parent.font.family
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            // Alternative Action Redirection
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 4

                Text {
                    text: "Don't have an account yet?"
                    font.pixelSize: 13
                    color: root.textMuted
                    font.family: "Inter"
                }

                Button {
                    id: signUpBtn
                    text: "Sign Up"
                    background: null

                    contentItem: Text {
                        text: parent.text
                        color: signUpBtn.hovered ? "#B45309" : root.accentColor
                        font.pixelSize: 13
                        font.bold: true
                        font.family: "Inter"
                    }
                }
            }
        }
    }

    // Clean App Footer
    Text {
        text: "© 2026 Karcha Tracker. Smart Student Financial Management Dashboard System."
        font.pixelSize: 11
        color: root.textMuted
        font.family: "Inter"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
    }
}