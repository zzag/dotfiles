import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk

from gi.repository import Gio


TITLE = 'ohai'


class MainWindow(Gtk.Window):

    def __init__(self):
        super().__init__(title=TITLE)
        self.props.border_width = 10
        self.set_default_size(800, 600)

        header_bar = Gtk.HeaderBar()
        header_bar.props.show_close_button = True
        header_bar.props.title = TITLE
        self.set_titlebar(header_bar)

        add_btn = Gtk.Button()
        add_icon = Gio.ThemedIcon(name='list-add-symbolic')
        add_btn.add(Gtk.Image.new_from_gicon(add_icon, Gtk.IconSize.BUTTON))
        add_btn.connect('clicked', self.on_add_clicked)
        header_bar.pack_end(add_btn)

        nav_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        Gtk.StyleContext.add_class(nav_box.get_style_context(), 'linked')
        header_bar.pack_start(nav_box)

        left_btn = Gtk.Button()
        left_btn.add(Gtk.Arrow(Gtk.ArrowType.LEFT))
        left_btn.connect('clicked', self.on_left_clicked)
        nav_box.add(left_btn)

        right_btn = Gtk.Button()
        right_btn.add(Gtk.Arrow(Gtk.ArrowType.RIGHT))
        right_btn.connect('clicked', self.on_right_clicked)
        nav_box.add(right_btn)

        fancy_lbl = Gtk.Label('hai')
        self.add(fancy_lbl)

    def on_left_clicked(self, widget):
        print('left')

    def on_right_clicked(self, widget):
        print('right')

    def on_add_clicked(self, widget):
        print('ohai')


window = MainWindow()
window.connect('delete-event', Gtk.main_quit)
window.show_all()
Gtk.main()
