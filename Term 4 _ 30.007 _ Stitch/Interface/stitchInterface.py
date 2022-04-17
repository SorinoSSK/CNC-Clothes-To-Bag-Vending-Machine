import sys
import os
import PyQt5.QtQuick
from PyQt5 import QtSerialPort
from PyQt5.QtCore import QUrl, QObject, QIODevice, pyqtSignal, pyqtSlot
from PyQt5.QtGui import QGuiApplication
from PyQt5.QtQml import QQmlApplicationEngine


class Bridge(QObject):

    arduinoRead = pyqtSignal(str, name='arduinoRead', arguments=['ardReply'])

    def __init__(self):
        super().__init__()
        self.reply = ""
        self.checked = False
        self.serial = QtSerialPort.QSerialPort(
            '/dev/ttyACM0',
            baudRate=QtSerialPort.QSerialPort.Baud115200,
            readyRead=self.receive
        )
        self.serial.open(QIODevice.ReadWrite)

    @pyqtSlot()
    def receive(self):
        while self.serial.canReadLine():
            text = self.serial.readLine().data().decode()
            text = text.rstrip('\r\n')
            self.reply = text

            self.arduinoRead.emit(text)

    @pyqtSlot(str)
    def send(self, inStr):
        self.serial.write(inStr.encode())
    
    @pyqtSlot(result = str)
    def returnStr(self):
        return self.reply



if __name__ == '__main__':
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    engine.quit.connect(app.quit)

    #Create insteance of the bridge object
    bridge = Bridge()

    #Expose the Python object to QML
    context = engine.rootContext()
    context.setContextProperty("con", bridge)

    filename = os.path.join(os.path.dirname(__file__), "pages/main.qml")
    url = QUrl.fromLocalFile(filename)
    engine.load(url)
    sys.exit(app.exec())