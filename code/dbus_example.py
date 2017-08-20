import sys
import signal
import dbus
import dbus.service

from PyQt5.QtCore import QCoreApplication
from dbus.mainloop.pyqt5 import DBusQtMainLoop


class MyService(dbus.service.Object):

    def __init__(self):
        bus_name = dbus.service.BusName('io.vulpes.MyService', bus=dbus.SessionBus())
        super().__init__(bus_name, '/io/vulpes/MyService')

    @dbus.service.method('io.vulpes.MyService',
                         in_signature='dd',
                         out_signature='d')
    def Add(self, a, b):
        return a + b

signal.signal(signal.SIGINT, signal.SIG_DFL)

DBusQtMainLoop(set_as_default=True)
app = QCoreApplication(sys.argv)
service = MyService()
app.exec_()
