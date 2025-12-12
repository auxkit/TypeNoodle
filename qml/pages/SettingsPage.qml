import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import TypeNoodle

Item {
    id: root

    signal closeRequested()

    // Folder dialog for selecting watch folders
    FolderDialog {
        id: folderDialog
        title: "Select Font Folder to Watch"
        onAccepted: {
            // Convert URL to path
            var path = selectedFolder.toString();
            // Remove file:// prefix
            if (path.startsWith("file://")) {
                path = path.substring(7);
            }
            // Decode URI components
            path = decodeURIComponent(path);
            Settings.addWatchFolder(path);
        }
    }

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
                    text: "Close"
                    onClicked: root.closeRequested()

                    background: Rectangle {
                        color: parent.hovered ? Theme.surface0 : Theme.cardBackground
                        radius: Theme.radiusMd
                    }

                    contentItem: Text {
                        text: parent.text
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
                                    checked: true
                                    enabled: false // Placeholder - theme switching not implemented yet

                                    indicator: Rectangle {
                                        width: 48
                                        height: 24
                                        radius: 12
                                        color: parent.checked ? Theme.primary : Theme.surface1

                                        Rectangle {
                                            x: parent.parent.checked ? parent.width - width - 2 : 2
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
                                from: 12
                                to: 144
                                value: 48

                                background: Rectangle {
                                    color: Theme.surface0
                                    radius: Theme.radiusMd
                                }

                                contentItem: TextInput {
                                    text: parent.value
                                    color: Theme.foreground
                                    font.pixelSize: Theme.fontSizeMd
                                    horizontalAlignment: Qt.AlignHCenter
                                    verticalAlignment: Qt.AlignVCenter
                                    readOnly: !parent.editable
                                }
                            }
                        }
                    }

                    // Watch Folders section
                    GroupBox {
                        Layout.fillWidth: true

                        background: Rectangle {
                            color: Theme.cardBackground
                            radius: Theme.radiusLg
                        }

                        label: Text {
                            text: "Watch Folders"
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeLg
                            font.bold: true
                            padding: Theme.spacingSm
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: Theme.spacingMd

                            Text {
                                text: "Add folders containing fonts that aren't installed on your system. These folders will be scanned for font files."
                                color: Theme.foregroundDim
                                font.pixelSize: Theme.fontSizeSm
                                wrapMode: Text.WordWrap
                                Layout.fillWidth: true
                            }

                            // Add folder button
                            Button {
                                text: "+ Add Folder"
                                onClicked: folderDialog.open()

                                background: Rectangle {
                                    color: parent.hovered ? Theme.primaryHover : Theme.primary
                                    radius: Theme.radiusMd
                                }

                                contentItem: Text {
                                    text: parent.text
                                    color: Theme.background
                                    font.pixelSize: Theme.fontSizeMd
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                            // List of watch folders
                            ListView {
                                id: watchFoldersList
                                Layout.fillWidth: true
                                Layout.preferredHeight: Math.min(contentHeight, 200)
                                visible: Settings.watchFolders.length > 0
                                clip: true
                                spacing: Theme.spacingXs

                                model: Settings.watchFolders

                                delegate: Rectangle {
                                    width: watchFoldersList.width
                                    height: 40
                                    color: Theme.surface0
                                    radius: Theme.radiusSm

                                    RowLayout {
                                        anchors.fill: parent
                                        anchors.leftMargin: Theme.spacingSm
                                        anchors.rightMargin: Theme.spacingSm
                                        spacing: Theme.spacingSm

                                        Text {
                                            text: modelData
                                            color: Theme.foreground
                                            font.pixelSize: Theme.fontSizeSm
                                            elide: Text.ElideMiddle
                                            Layout.fillWidth: true
                                        }

                                        Button {
                                            text: "×"
                                            implicitWidth: 28
                                            implicitHeight: 28
                                            onClicked: Settings.removeWatchFolder(modelData)

                                            background: Rectangle {
                                                color: parent.hovered ? Theme.red : Theme.surface1
                                                radius: Theme.radiusSm
                                            }

                                            contentItem: Text {
                                                text: parent.text
                                                color: Theme.foreground
                                                font.pixelSize: Theme.fontSizeLg
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                        }
                                    }
                                }
                            }

                            // Empty state
                            Text {
                                text: "No watch folders added yet"
                                color: Theme.foregroundDim
                                font.pixelSize: Theme.fontSizeSm
                                font.italic: true
                                visible: Settings.watchFolders.length === 0
                            }

                            // Rescan button
                            Button {
                                text: "Rescan Folders"
                                visible: Settings.watchFolders.length > 0
                                onClicked: {
                                    if (FontManager) {
                                        FontManager.refreshFonts();
                                    }
                                }

                                background: Rectangle {
                                    color: parent.hovered ? Theme.surface1 : Theme.surface0
                                    radius: Theme.radiusMd
                                }

                                contentItem: Text {
                                    text: parent.text
                                    color: Theme.foreground
                                    font.pixelSize: Theme.fontSizeMd
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
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
