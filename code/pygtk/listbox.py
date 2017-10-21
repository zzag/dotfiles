import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk


def build_btn_row(text, spacing=10):
    label = Gtk.Label(text)
    switch = Gtk.Switch()
    box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,
                  spacing=spacing)
    box.pack_start(label, expand=False,
                   fill=False, padding=0)
    box.pack_end(switch, expand=False,
                 fill=False, padding=0)
    row = Gtk.ListBoxRow()
    row.add(box)
    return row


class MainWindow(Gtk.Window):

    def __init__(self):
        super().__init__(title='Random window')
        self.set_border_width(10)
        self.set_default_size(640, 480)

        list_box = Gtk.ListBox()
        list_box.set_selection_mode(Gtk.SelectionMode.NONE)
        list_box.add(build_btn_row('Minimize'))
        list_box.add(build_btn_row('Maximize'))
        list_box.add(build_btn_row('Close'))
        self.add(list_box)


w = MainWindow()
w.connect('delete-event', Gtk.main_quit)
w.show_all()
Gtk.main()
