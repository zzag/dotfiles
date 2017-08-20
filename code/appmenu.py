import sys
import signal
import dbus
import dbus.service

from PyQt5.QtCore import QCoreApplication
from dbus.mainloop.pyqt5 import DBusQtMainLoop


SERVICE_NAME = 'com.canonical.AppMenu.Registrar'
OBJECT_PATH = '/com/canonical/AppMenu/Registrar'


class AppMenuService(dbus.service.Object):

    def __init__(self):
        bus_name = dbus.service.BusName(SERVICE_NAME, bus=dbus.SessionBus())
        super().__init__(bus_name, OBJECT_PATH)
        self.menus = {}

    @dbus.service.method('com.canonical.AppMenu.Registrar',
                         in_signature='uo',
                         sender_keyword='service')
    def RegisterWindow(self, windowId, menuObjectPath, service):
        self.menus[windowId] = (service, menuObjectPath)

    @dbus.service.method('com.canonical.AppMenu.Registrar',
                         in_signature='u')
    def UnregisterWindow(self, windowId):
        del self.menus[windowId]

    @dbus.service.method('com.canonical.AppMenu.Registrar',
                         in_signature='u',
                         out_signature='so')
    def GetMenuForWindow(self, windowId):
        if windowId in self.windows:
            service, object_path = self.menus[windowId]
            return service, object_path


signal.signal(signal.SIGINT, signal.SIG_DFL)

DBusQtMainLoop(set_as_default=True)
app = QCoreApplication(sys.argv)
service = AppMenuService()
app.exec_()
