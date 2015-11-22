import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }
   
    Image {
	source: "file:///ipfs/QmavE42xtK1VovJFVTVkCR5Jdf761QWtxmvak9Zx718TVr/files/2012/07/bali-money-as-garnish-pigs-ear-close4.jpg"
    }

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}

