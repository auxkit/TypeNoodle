import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

Rectangle {
    id: root

    property string fontFamily: "Sans Serif"
    property string fontStyle: "Regular"
    property string displayName: fontFamily
    property string format: "TrueType"
    property bool active: true

    signal clicked()
    signal toggleActivation()

    width: 300
    height: 200
    color: Theme.cardBackground
    radius: Theme.radiusLg

    border.color: mouseArea.containsMouse ? Theme.primary : "transparent"
    border.width: 2

    Behavior on border.color {
        ColorAnimation { duration: Theme.transitionFast }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingMd
        spacing: Theme.spacingSm

        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingSm

            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: root.active ? Theme.success : Theme.foregroundDim
            }

            Text {
                text: root.format
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeXs
                Layout.fillWidth: true
            }

            Button {
                text: root.active ? "Deactivate" : "Activate"
                flat: true
                implicitHeight: 24

                background: Rectangle {
                    color: parent.hovered ? Theme.surface1 : Theme.surface0
                    radius: Theme.radiusSm
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeXs
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    root.toggleActivation()
                    mouse.accepted = true
                }
            }
        }

        // Preview text
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                anchors.centerIn: parent
                text: "Aa"
                color: Theme.foreground
                font.family: root.fontFamily
                font.styleName: root.fontStyle
                font.pixelSize: 72
            }
        }

        // Footer
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 2

            Text {
                text: root.displayName
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Text {
                text: root.fontStyle
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeSm
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }
    }
}
