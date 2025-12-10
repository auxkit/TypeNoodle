import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    signal refreshRequested()
    signal settingsRequested()

    color: Theme.backgroundAlt
    height: 60

    RowLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingMd
        spacing: Theme.spacingMd

        // Logo and title
        RowLayout {
            spacing: Theme.spacingSm

            Text {
                text: "\uf031" // Font icon
                color: Theme.primary
                font.pixelSize: Theme.fontSizeXxl
                font.family: "Font Awesome 6 Free"
            }

            Text {
                text: "TypeNoodle"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeXl
                font.bold: true
            }
        }

        Item { Layout.fillWidth: true }

        // Action buttons
        RowLayout {
            spacing: Theme.spacingSm

            Button {
                text: "Refresh"
                flat: true

                background: Rectangle {
                    color: parent.hovered ? Theme.cardBackground : "transparent"
                    radius: Theme.radiusMd
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeMd
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.refreshRequested()
            }

            Button {
                text: "Settings"
                flat: true

                background: Rectangle {
                    color: parent.hovered ? Theme.cardBackground : "transparent"
                    radius: Theme.radiusMd
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeMd
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.settingsRequested()
            }
        }
    }
}
