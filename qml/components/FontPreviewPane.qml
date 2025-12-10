import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property string fontFamily: "Sans Serif"
    property string fontStyle: "Regular"
    property string displayName: fontFamily
    property string format: "TrueType"
    property string filePath: ""
    property int previewSize: 48

    color: Theme.background
    visible: fontFamily !== ""

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingLg
        spacing: Theme.spacingLg

        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingMd

            ColumnLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingXs

                Text {
                    text: root.displayName
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeXxl
                    font.bold: true
                }

                Text {
                    text: root.fontStyle + " • " + root.format
                    color: Theme.foregroundDim
                    font.pixelSize: Theme.fontSizeMd
                }

                Text {
                    text: root.filePath
                    color: Theme.foregroundDim
                    font.pixelSize: Theme.fontSizeSm
                    elide: Text.ElideMiddle
                    Layout.fillWidth: true
                }
            }

            Button {
                text: "Close"
                onClicked: root.visible = false

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

        // Size slider
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingMd

            Text {
                text: "Size:"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
            }

            Slider {
                id: sizeSlider
                Layout.fillWidth: true
                from: 12
                to: 144
                value: root.previewSize
                onValueChanged: root.previewSize = value

                background: Rectangle {
                    x: sizeSlider.leftPadding
                    y: sizeSlider.topPadding + sizeSlider.availableHeight / 2 - height / 2
                    width: sizeSlider.availableWidth
                    height: 4
                    radius: 2
                    color: Theme.surface1

                    Rectangle {
                        width: sizeSlider.visualPosition * parent.width
                        height: parent.height
                        color: Theme.primary
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: sizeSlider.leftPadding + sizeSlider.visualPosition * (sizeSlider.availableWidth - width)
                    y: sizeSlider.topPadding + sizeSlider.availableHeight / 2 - height / 2
                    width: 16
                    height: 16
                    radius: 8
                    color: sizeSlider.pressed ? Theme.primaryHover : Theme.primary
                }
            }

            Text {
                text: Math.round(root.previewSize) + "px"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
                Layout.minimumWidth: 50
            }
        }

        // Preview text input
        TextField {
            id: previewInput
            Layout.fillWidth: true
            placeholderText: "Type to preview..."
            text: "The quick brown fox jumps over the lazy dog"

            background: Rectangle {
                color: Theme.cardBackground
                radius: Theme.radiusMd
            }

            color: Theme.foreground
            font.pixelSize: Theme.fontSizeMd
            padding: Theme.spacingMd
        }

        // Preview area
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            Rectangle {
                width: parent.width
                implicitHeight: previewText.implicitHeight + Theme.spacingXl * 2
                color: Theme.cardBackground
                radius: Theme.radiusLg

                Text {
                    id: previewText
                    anchors.fill: parent
                    anchors.margins: Theme.spacingXl
                    text: previewInput.text
                    color: Theme.foreground
                    font.family: root.fontFamily
                    font.styleName: root.fontStyle
                    font.pixelSize: root.previewSize
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}
