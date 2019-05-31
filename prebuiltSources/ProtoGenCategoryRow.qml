import QtQuick 2.9
import QtQuick.Controls 2.3

Row {
    property bool synchro: true
    spacing: 10
    Binding {
        target: header
        property: "inSync"
        value: synchro
    }
}
