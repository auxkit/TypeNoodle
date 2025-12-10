import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property alias text: searchField.text
    property alias placeholderText: searchField.placeholderText

    signal textChanged()

    color: Theme.cardBackground
    radius: Theme.radiusMd
    height: 40

    RowLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingSm
        spacing: Theme.spacingSm

        Text {
            text: "\uf002" // Search icon (Font Awesome)
            color: Theme.foregroundDim
            font.pixelSize: Theme.fontSizeMd
            font.family: "Font Awesome 6 Free"
        }

        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Search fonts..."

            background: Rectangle {
                color: "transparent"
            }

            color: Theme.foreground
            font.pixelSize: Theme.fontSizeMd
            selectByMouse: true

            onTextChanged: root.textChanged()
        }

        Button {
            visible: searchField.text.length > 0
            text: "\u00d7" // × symbol
            flat: true

            background: Rectangle {
                color: parent.hovered ? Theme.surface1 : "transparent"
                radius: Theme.radiusSm
            }

            contentItem: Text {
                text: parent.text
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeLg
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: searchField.text = ""
        }
    }
}
