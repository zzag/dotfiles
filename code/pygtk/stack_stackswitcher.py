import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk


DEFAULT_WINDOW_TITLE = 'Random window'
DEFAULT_WINDOW_SIZE = (640, 480)


class MainWindow(Gtk.Window):

    def __init__(self):
        super().__init__(title=DEFAULT_WINDOW_TITLE)
        self.set_default_size(*DEFAULT_WINDOW_SIZE)
        self.set_border_width(10)

        main_area = Gtk.Stack()
        main_area.set_transition_type(Gtk.StackTransitionType.SLIDE_LEFT_RIGHT)
        main_area.set_transition_duration(666)

        main_area.add_titled(self._build_tab1(), 'tab1', 'Tab 1')
        main_area.add_titled(self._build_tab2(), 'tab2', 'Tab 2')
        main_area.add_titled(self._build_tab3(), 'tab3', 'Tab 3')

        switcher = Gtk.StackSwitcher()
        switcher.set_stack(main_area)
        switcher_align = Gtk.Alignment(xalign=0.5, yalign=0.0,
                                       xscale=0.0, yscale=0.0)
        switcher_align.add(switcher)

        outer_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        outer_box.pack_start(switcher_align, expand=False, 
                             fill=False, padding=0)
        outer_box.pack_start(main_area, expand=True, fill=True, padding=0)
        self.add(outer_box)

    def _build_tab1(self):
        welcome_lbl = Gtk.Label('Welcome to the tab1 space.')
        return welcome_lbl

    def _build_tab2(self):
        hai_lbl = Gtk.Label('Hai')
        return hai_lbl

    def _build_tab3(self):
        exit_lbl = Gtk.Label('You did it! Go away!')
        return exit_lbl


w = MainWindow()
w.connect('delete-event', Gtk.main_quit)
w.show_all()
Gtk.main()
