import QtQuick 2.9
import QtQuick.Controls 2.3

Row {
    id: control

    property double val: 0
    property int precision: 4

    property alias label: protoGenNumberLabel.text
    property alias units: protoGenNumberUnits.text
    property string comment: ""

    readonly property int fontSize: 10

    width: parent.width
    height: 24
    spacing: 10

    Label {
        id: protoGenNumberLabel
        font.pointSize: control.fontSize
        width: control.width/3-control.spacing
        height: control.height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        ToolTip.text: control.comment
        ToolTip.visible: ("" !== control.comment) ? mouseArea.containsMouse : false
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    TextField {
        id: protoGenNumberValue
        text: control.val.toFixed(control.precision)
        validator: DoubleValidator {decimals: control.precision}
        font.pointSize: fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignRight
        rightPadding: 5
        topPadding: 5
        bottomPadding: 5
        width: parent.width/2-parent.spacing
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

    Label {
        id: protoGenNumberUnits
        font.pointSize: control.fontSize
        width: parent.width/6-parent.spacing
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}