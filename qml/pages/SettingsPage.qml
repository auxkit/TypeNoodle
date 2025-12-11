import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

Item {
    id: root

    signal closeRequested()

    Rectangle {
        anchors.fill: parent
        color: Theme.background

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLg
            spacing: Theme.spacingLg

            // Header
            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: "Settings"
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeXxl
                    font.bold: true
                    Layout.fillWidth: true
                }

                Button {
                    id: closeButton
                    text: "Close"
                    onClicked: root.closeRequested()

                    background: Rectangle {
                        color: closeButton.hovered ? Theme.surface0 : Theme.cardBackground
                        radius: Theme.radiusMd
                    }

                    contentItem: Text {
                        text: closeButton.text
                        color: Theme.foreground
                        font.pixelSize: Theme.fontSizeMd
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // Settings content
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: parent.width
                    spacing: Theme.spacingMd

                    // Appearance section
                    GroupBox {
                        Layout.fillWidth: true

                        background: Rectangle {
                            color: Theme.cardBackground
                            radius: Theme.radiusLg
                        }

                        label: Text {
                            text: "Appearance"
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeLg
                            font.bold: true
                            padding: Theme.spacingSm
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: Theme.spacingMd

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    text: "Dark Theme"
                                    color: Theme.foreground
                                    font.pixelSize: Theme.fontSizeMd
                                    Layout.fillWidth: true
                                }

                                Switch {
                                    id: themeSwitch
                                    checked: true
                                    enabled: false // Placeholder - theme switching not implemented yet

                                    indicator: Rectangle {
                                        width: 48
                                        height: 24
                                        radius: 12
                                        color: themeSwitch.checked ? Theme.primary : Theme.surface1

                                        Rectangle {
                                            x: themeSwitch.checked ? parent.width - width - 2 : 2
                                            y: 2
                                            width: 20
                                            height: 20
                                            radius: 10
                                            color: Theme.foreground

                                            Behavior on x {
                                                NumberAnimation { duration: Theme.transitionFast }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Preview section
                    GroupBox {
                        Layout.fillWidth: true

                        background: Rectangle {
                            color: Theme.cardBackground
                            radius: Theme.radiusLg
                        }

                        label: Text {
                            text: "Preview"
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeLg
                            font.bold: true
                            padding: Theme.spacingSm
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: Theme.spacingMd

                            Text {
                                text: "Default Preview Text"
                                color: Theme.foreground
                                font.pixelSize: Theme.fontSizeMd
                            }

                            TextField {
                                Layout.fillWidth: true
                                placeholderText: "The quick brown fox jumps over the lazy dog"
                                text: "The quick brown fox jumps over the lazy dog"

                                background: Rectangle {
                                    color: Theme.surface0
                                    radius: Theme.radiusMd
                                }

                                color: Theme.foreground
                                font.pixelSize: Theme.fontSizeMd
                                padding: Theme.spacingSm
                            }

                            Text {
                                text: "Default Preview Size"
                                color: Theme.foreground
                                font.pixelSize: Theme.fontSizeMd
                            }

                            SpinBox {
                                id: iconSizeSpinBox
                                from: 12
                                to: 144
                                value: 48

                                background: Rectangle {
                                    color: Theme.surface0
                                    radius: Theme.radiusMd
                                }

                                contentItem: TextInput {
                                    text: iconSizeSpinBox.value
                                    color: Theme.foreground
                                    font.pixelSize: Theme.fontSizeMd
                                    horizontalAlignment: Qt.AlignHCenter
                                    verticalAlignment: Qt.AlignVCenter
                                    readOnly: !iconSizeSpinBox.editable
                                }
                            }
                        }
                    }

                    // About section
                    GroupBox {
                        Layout.fillWidth: true

                        background: Rectangle {
                            color: Theme.cardBackground
                            radius: Theme.radiusLg
                        }

                        label: Text {
                            text: "About"
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeLg
                            font.bold: true
                            padding: Theme.spacingSm
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: Theme.spacingSm

                            Text {
                                text: "TypeNoodle v1.0.0"
                                color: Theme.foreground
                                font.pixelSize: Theme.fontSizeMd
                            }

                            Text {
                                text: "Cross-platform desktop font manager"
                                color: Theme.foregroundDim
                                font.pixelSize: Theme.fontSizeSm
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
