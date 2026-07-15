/*
This is a UI file (.ui.qml) for the Karcha Tracker - Nepali Student Expense Manager
Modern, aesthetic design with teal and warm amber colors
Optimized for Qt Quick Controls - 100% Declarative & Form Compliant
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 1920
    height: 1080
    color: "#F9FAFB"

    // Color Palette
    readonly property color primaryColor: "#0F766E"
    readonly property color accentColor: "#D97706"
    readonly property color successColor: "#059669"
    readonly property color warningColor: "#F59E0B"
    readonly property color dangerColor: "#DC2626"
    readonly property color cardBg: "#FFFFFF"
    readonly property color textDark: "#1F2937"
    readonly property color textMuted: "#6B7280"
    readonly property color borderColor: "#E5E7EB"

    // Properties for data binding
    property alias expenseDescription: descriptionField.text
    property alias expenseAmount: amountField.text
    property alias categoryDropdown: categoryCombo.currentIndex
    property alias addExpenseButton: addExpenseBtn
    property alias budgetField: budgetInput.text
    property alias budgetButton: budgetBtn
    property alias deleteButton: deleteBtn

    // ==========================================
    // HEADER / TOP NAVIGATION BAR
    // ==========================================
    Rectangle {
        id: headerBar
        width: parent.width
        height: 80
        color: "#FFFFFF"
        border.color: root.borderColor
        border.width: 1
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            Column {
                Layout.fillWidth: true
                spacing: 4

                Text {
                    text: "खर्च ट्र्याकर"
                    font.pixelSize: 28
                    font.bold: true
                    color: root.primaryColor
                    font.family: "Poppins"
                }

                Text {
                    text: "Karcha Tracker - Smart Spending for Smart Students"
                    font.pixelSize: 12
                    color: root.textMuted
                    font.family: "Inter"
                }
            }

            Column {
                spacing: 4
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                Text {
                    text: "Current Month"
                    font.pixelSize: 11
                    color: root.textMuted
                    font.family: "Inter"
                    Layout.alignment: Qt.AlignRight
                }

                Text {
                    text: "₹ 0.00"
                    font.pixelSize: 24
                    font.bold: true
                    color: root.textDark
                    font.family: "Poppins"
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
    }

    // ==========================================
    // MAIN CONTENT AREA (SCROLLABLE)
    // ==========================================
    ScrollView {
        id: mainScrollView
        anchors.top: headerBar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        clip: true

        ColumnLayout {
            width: mainScrollView.width - 20
            spacing: 24

            // ==========================================
            // SUMMARY CARDS (3 Column Grid)
            // ==========================================
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 140
                spacing: 20

                // Card 1: Budget Limit
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: root.cardBg
                    radius: 12
                    border.color: root.borderColor
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 20

                        Column {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                text: "BUDGET LIMIT"
                                font.pixelSize: 12
                                font.bold: true
                                color: root.textMuted
                                font.family: "Inter"
                            }

                            Text {
                                text: "₹ 50,000.00"
                                font.pixelSize: 32
                                font.bold: true
                                color: root.textDark
                                font.family: "Poppins"
                            }
                        }

                        Text {
                            text: "💰"
                            font.pixelSize: 40
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        }
                    }
                }

                // Card 2: This Month Spent
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: root.cardBg
                    radius: 12
                    border.color: root.borderColor
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 20

                        Column {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                text: "THIS MONTH SPENT"
                                font.pixelSize: 12
                                font.bold: true
                                color: root.textMuted
                                font.family: "Inter"
                            }

                            Text {
                                text: "₹ 0.00"
                                font.pixelSize: 32
                                font.bold: true
                                color: root.textDark
                                font.family: "Poppins"
                            }
                        }

                        Text {
                            text: "📊"
                            font.pixelSize: 40
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        }
                    }
                }

                // Card 3: Budget Status
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: root.cardBg
                    radius: 12
                    border.color: root.borderColor
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 20

                        Column {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                text: "BUDGET STATUS"
                                font.pixelSize: 12
                                font.bold: true
                                color: root.textMuted
                                font.family: "Inter"
                            }

                            Text {
                                text: "0%"
                                font.pixelSize: 32
                                font.bold: true
                                color: root.successColor
                                font.family: "Poppins"
                            }
                        }

                        Text {
                            text: "✅"
                            font.pixelSize: 40
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        }
                    }
                }
            }

            // ==========================================
            // MAIN CONTENT GRID (Charts + Forms)
            // ==========================================
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 740
                spacing: 24

                // LEFT COLUMN: Charts Section
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width * 0.65
                    Layout.fillHeight: true
                    spacing: 20

                    // Category Breakdown Chart
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 360
                        color: root.cardBg
                        radius: 12
                        border.color: root.borderColor
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 12

                            RowLayout {
                                spacing: 8
                                Text { text: "📈"; font.pixelSize: 20 }
                                Text {
                                    text: "Spending by Category"
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: root.textDark
                                    font.family: "Poppins"
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "#F3F4F6"
                                radius: 8
                                Text {
                                    anchors.centerIn: parent
                                    text: "Chart visualization area\n(Connect to backend for data)"
                                    horizontalAlignment: Text.AlignHCenter
                                    color: root.textMuted
                                    font.pixelSize: 14
                                    font.family: "Inter"
                                }
                            }
                        }
                    }

                    // Daily Spending Trend
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 360
                        color: root.cardBg
                        radius: 12
                        border.color: root.borderColor
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 12

                            RowLayout {
                                spacing: 8
                                Text { text: "📈"; font.pixelSize: 20 }
                                Text {
                                    text: "Daily Spending Trend"
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: root.textDark
                                    font.family: "Poppins"
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                color: "#F3F4F6"
                                radius: 8
                                Text {
                                    anchors.centerIn: parent
                                    text: "Line chart area\n(Connect to backend for data)"
                                    horizontalAlignment: Text.AlignHCenter
                                    color: root.textMuted
                                    font.pixelSize: 14
                                    font.family: "Inter"
                                }
                            }
                        }
                    }
                }

                // RIGHT COLUMN: Forms Section
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.preferredWidth: parent.width * 0.35
                    Layout.fillHeight: true
                    spacing: 20

                    // Add Expense Form
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 450
                        color: root.cardBg
                        radius: 12
                        border.color: root.borderColor
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 16

                            RowLayout {
                                spacing: 8
                                Text { text: "➕"; font.pixelSize: 20 }
                                Text {
                                    text: "Add Expense"
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: root.textDark
                                    font.family: "Poppins"
                                }
                            }

                            // Description Field
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 6
                                Text { text: "Description"; font.pixelSize: 12; font.bold: true; color: root.textMuted; font.family: "Inter" }
                                TextField {
                                    id: descriptionField
                                    Layout.fillWidth: true
                                    height: 40
                                    placeholderText: "What did you buy?"
                                    font.family: "Inter"
                                    font.pixelSize: 13
                                    color: root.textDark
                                    leftPadding: 10
                                    background: Rectangle {
                                        color: "#F3F4F6"
                                        radius: 8
                                        border.color: descriptionField.activeFocus ? root.primaryColor : root.borderColor
                                        border.width: 1
                                    }
                                }
                            }

                            // Amount Field
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 6
                                Text { text: "Amount (₹)"; font.pixelSize: 12; font.bold: true; color: root.textMuted; font.family: "Inter" }
                                TextField {
                                    id: amountField
                                    Layout.fillWidth: true
                                    height: 40
                                    placeholderText: "0.00"
                                    font.family: "Inter"
                                    font.pixelSize: 13
                                    color: root.textDark
                                    leftPadding: 10
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                    background: Rectangle {
                                        color: "#F3F4F6"
                                        radius: 8
                                        border.color: amountField.activeFocus ? root.primaryColor : root.borderColor
                                        border.width: 1
                                    }
                                }
                            }

                            // Category Dropdown
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 6
                                Text { text: "Category"; font.pixelSize: 12; font.bold: true; color: root.textMuted; font.family: "Inter" }
                                ComboBox {
                                    id: categoryCombo
                                    Layout.fillWidth: true
                                    height: 40
                                    font.family: "Inter"
                                    font.pixelSize: 13
                                    model: [
                                        "🍽️ Food & Dining",
                                        "🚗 Transport",
                                        "🎬 Entertainment",
                                        "💡 Utilities",
                                        "🏠 Housing",
                                        "📚 Education",
                                        "⚕️ Health",
                                        "📌 Other"
                                    ]
                                    background: Rectangle {
                                        color: "#F3F4F6"
                                        radius: 8
                                        border.color: root.borderColor
                                        border.width: 1
                                    }
                                }
                            }

                            // Add Expense Button
                            Button {
                                id: addExpenseBtn
                                Layout.fillWidth: true
                                height: 44
                                text: "Log Expense"
                                font.pixelSize: 14
                                font.bold: true
                                font.family: "Inter"
                                background: Rectangle {
                                    color: addExpenseBtn.hovered ? "#0D5F5A" : root.primaryColor
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
                        }
                    }

                    // Budget Settings
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 270
                        color: root.cardBg
                        radius: 12
                        border.color: root.borderColor
                        border.width: 1

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 16

                            Text {
                                text: "Budget Settings"
                                font.pixelSize: 18
                                font.bold: true
                                color: root.textDark
                                font.family: "Poppins"
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 6
                                Text { text: "Monthly Budget (₹)"; font.pixelSize: 12; font.bold: true; color: root.textMuted; font.family: "Inter" }
                                TextField {
                                    id: budgetInput
                                    Layout.fillWidth: true
                                    height: 40
                                    placeholderText: "50000"
                                    text: "50000"
                                    font.family: "Inter"
                                    font.pixelSize: 13
                                    color: root.textDark
                                    leftPadding: 10
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                    background: Rectangle {
                                        color: "#F3F4F6"
                                        radius: 8
                                        border.color: budgetInput.activeFocus ? root.primaryColor : root.borderColor
                                        border.width: 1
                                    }
                                }
                            }

                            Button {
                                id: budgetBtn
                                Layout.fillWidth: true
                                height: 44
                                text: "Save Budget"
                                font.pixelSize: 14
                                font.bold: true
                                font.family: "Inter"
                                background: Rectangle {
                                    color: budgetBtn.hovered ? "#B45309" : root.accentColor
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
                        }
                    }
                }
            }

            // ==========================================
            // EXPENSE HISTORY TABLE
            // ==========================================
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 400
                color: root.cardBg
                radius: 12
                border.color: root.borderColor
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 12

                    Text {
                        text: "Recent Expenses"
                        font.pixelSize: 18
                        font.bold: true
                        color: root.textDark
                        font.family: "Poppins"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: root.borderColor
                    }

                    // Table Header
                    RowLayout {
                        Layout.fillWidth: true
                        height: 40
                        spacing: 20

                        Text {
                            text: "Date"
                            font.pixelSize: 12
                            font.bold: true
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                        }

                        Text {
                            text: "Description"
                            font.pixelSize: 12
                            font.bold: true
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.fillWidth: true
                        }

                        Text {
                            text: "Category"
                            font.pixelSize: 12
                            font.bold: true
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 120
                        }

                        Text {
                            text: "Amount"
                            font.pixelSize: 12
                            font.bold: true
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                            horizontalAlignment: Text.AlignRight
                        }

                        Text {
                            text: "Action"
                            font.pixelSize: 12
                            font.bold: true
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 60
                            horizontalAlignment: Text.AlignCenter
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: root.borderColor
                    }

                    // Sample Row 1
                    RowLayout {
                        Layout.fillWidth: true
                        height: 50
                        spacing: 20

                        Text {
                            text: "Jun 28, 2026"
                            font.pixelSize: 13
                            color: root.textDark
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                        }

                        Text {
                            text: "Supermarket Groceries"
                            font.pixelSize: 13
                            color: root.textDark
                            font.family: "Inter"
                            Layout.fillWidth: true
                        }

                        Text {
                            text: "🍽️ Food"
                            font.pixelSize: 13
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 120
                        }

                        Text {
                            text: "₹ 650.00"
                            font.pixelSize: 13
                            font.bold: true
                            color: root.textDark
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                            horizontalAlignment: Text.AlignRight
                        }

                        Button {
                            id: deleteBtn
                            width: 30
                            height: 30
                            text: "🗑️"
                            font.pixelSize: 16
                            Layout.preferredWidth: 60
                            Layout.alignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: deleteBtn.hovered ? "#FEE2E2" : "transparent"
                                radius: 4
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: root.borderColor
                    }

                    // Sample Row 2
                    RowLayout {
                        Layout.fillWidth: true
                        height: 50
                        spacing: 20

                        Text {
                            text: "Jun 27, 2026"
                            font.pixelSize: 13
                            color: root.textDark
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                        }

                        Text {
                            text: "Premium Gas Fillup"
                            font.pixelSize: 13
                            color: root.textDark
                            font.family: "Inter"
                            Layout.fillWidth: true
                        }

                        Text {
                            text: "🚗 Transport"
                            font.pixelSize: 13
                            color: root.textMuted
                            font.family: "Inter"
                            Layout.preferredWidth: 120
                        }

                        Text {
                            text: "₹ 420.00"
                            font.pixelSize: 13
                            font.bold: true
                            color: root.textDark
                            font.family: "Inter"
                            Layout.preferredWidth: 100
                            horizontalAlignment: Text.AlignRight
                        }

                        Button {
                            id: deleteBtn2
                            width: 30
                            height: 30
                            text: "🗑️"
                            font.pixelSize: 16
                            Layout.preferredWidth: 60
                            Layout.alignment: Qt.AlignHCenter
                            background: Rectangle {
                                color: deleteBtn2.hovered ? "#FEE2E2" : "transparent"
                                radius: 4
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 1
                        color: root.borderColor
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}