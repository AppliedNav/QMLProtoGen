import QtQuick 2.9
import QtQuick.Controls 2.3

Column {
    id: control

    property alias hasLabel: controlRow.visible
    property double val: 0
    property int precision: 4

    property alias label: protoGenNumberLabel.text
    property alias units: protoGenNumberUnits.text
    property string comment: ""

    readonly property int fontSize: 10

    height: protoGenNumberLabel.height + protoGenNumberValue.height + control.spacing
    spacing: controlRow.visible ? 5 : 0

    Item {
        height: controlRow.visible ? 0 : 10
    }

    Row {
        id: controlRow
        spacing: 5
        width: control.width
        height: visible ? childrenRect.height : 0
        Label {
            id: protoGenNumberLabel
            clip: true
            font.pointSize: control.fontSize
            width: control.width - protoGenNumberUnits.width - controlRow.spacing
            height: controlRow.visible ? 24 : 0
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            ToolTip {
                text: control.comment
                visible: ("" !== control.comment) ? mouseArea.containsMouse : false
            }
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
        }
        Label {
            id: protoGenNumberUnits
            clip: true
            font.pointSize: control.fontSize
            width: ('' !== protoGenNumberUnits.text) ? 0.3 * control.width : 0
            height: protoGenNumberLabel.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
            MouseArea {
                id: unitsMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
            ToolTip {
                text: protoGenNumberUnits.text
                visible: protoGenNumberUnits.truncated && unitsMouseArea.containsMouse
            }
        }
    }

    TextField {
        id: protoGenNumberValue
        anchors {
            left: parent.left
            leftMargin: 5
            right: parent.right
            rightMargin: 5
        }
        text: control.val.toFixed(control.precision)
        validator: DoubleValidator {decimals: control.precision}
        font.pointSize: fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        rightPadding: 5
        topPadding: 5
        bottomPadding: 5
        onAccepted: {
            control.val = text
            text = control.val.toFixed(control.precision)
            parent.parent.synchro = false
        }
        background: Rectangle {
            anchors.fill: parent
            color: protoGenNumberValue.focus ? "gray" : "transparent"
            border.color: protoGenNumberValue.focus ? "white" : "gray"
        }
    }
}
