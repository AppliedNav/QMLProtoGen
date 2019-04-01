import QtQuick 2.9
import QtQuick.Controls 2.3

Row {
    id: control

    property double val: 0
    property int minval: 0
    property int maxval: 100
    property int step: 1

    property alias label: protoGenSpinLabel.text
    property alias units: protoGenSpinUnits.text
    property string comment: ""

    readonly property int fontSize: 10

    width: parent.width
    height: 24
    spacing: 10

    Label {
        id: protoGenSpinLabel
        font.pointSize: control.fontSize
        width: parent.width/3-4
        height: parent.height
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

    SpinBox {
        from: control.minval
        to: control.maxval
        stepSize: control.step
        value: control.val
        width: parent.width/2-4
        height: parent.height
        onValueChanged: {
            parent.parent.synchro = false
        }
    }

    Label {
        id: protoGenSpinUnits
        font.pointSize: control.fontSize
        width: parent.width/6-4
        height: parent.height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
