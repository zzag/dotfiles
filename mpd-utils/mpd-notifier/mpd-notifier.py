#!/usr/bin/python2

import argparse
import pynotify
import os

from mpd import MPDClient
from select import select


dirname = os.path.abspath(os.path.dirname(__file__))

MPD_NAME = 'MPD'
MPD_HOST = 'localhost'
MPD_PORT = 6600
DEFAULT_ICON = os.path.join(dirname, 'player.png')
DEFAULT_LIBRARY_DIR = os.path.join(os.environ['HOME'], 'Music')
DEFAULT_COVER_FILES = ('cover.png', 'cover.jpg')


def get_album_art(library_dir, song, cover_files):
    track_filename = os.path.join(library_dir, song['file'])
    track_dir = os.path.dirname(track_filename)
    for cover_file in cover_files:
        path = os.path.join(track_dir, cover_file)
        if os.path.isfile(path):
            return path
    return None


class MPDNotifier:

    def __init__(self, host, port, library_dir, cover_files):
        self.library_dir = library_dir
        self.cover_files = cover_files
        self.client = MPDClient()
        self.client.connect(host, port)
        self.last_status = self.client.status()

    def run_main_loop(self):
        self.client.send_idle()
        while True:
            select([self.client], [], [])
            events = self.client.fetch_idle()
            if 'player' in events:
                status = self.client.status()
                if status['state'] == 'pause':
                    self.handle_pause(status)
                elif status['state'] == 'stop':
                    self.handle_stop(status)
                elif status['songid'] != self.last_status['songid']:
                    self.handle_track(status)
                self.last_status = status
            self.client.send_idle()

    def handle_pause(self, status):
        self.notify('Paused')

    def handle_stop(self, status):
        # We need to set songid None in order to show song title and artist
        # next time when mpd will change its state back to 'play'.
        status['songid'] = None
        self.notify('Stopped')

    def handle_track(self, status):
        song = self.client.currentsong()
        title = song.get('title', 'Unknown')
        artist = song.get('artist', 'Unknown')

        message = '{title}<br/>{artist}'.format(title=title, artist=artist)
        icon = get_album_art(self.library_dir, song, self.cover_files)
        self.notify(message, icon)

    def notify(self, message, icon=None):
        summary = '<b>%s</b>' % MPD_NAME
        if icon is None:
            icon = DEFAULT_ICON
        notification = pynotify.Notification(summary, message, icon)
        notification.show()


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--host',
                        help='MPD host', default=MPD_HOST)
    parser.add_argument('--port',
                        help='MPD port', default=MPD_PORT)
    parser.add_argument('--library-dir',
                        help='Path to music library',
                        default=DEFAULT_LIBRARY_DIR)
    parser.add_argument('--cover-files', nargs='+',
                        help='List of cover filenames',
                        default=DEFAULT_COVER_FILES)
    args = vars(parser.parse_args())
    return args


def main():
    args = parse_args()

    pynotify.init('mpd-notifier')
    notifier = MPDNotifier(**args)

    try:
        notifier.run_main_loop()
    except KeyboardInterrupt:
        notifier.client.disconnect()


if __name__ == '__main__':
    main()
