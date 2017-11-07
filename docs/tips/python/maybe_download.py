from urllib import urlretrieve
import os


def maybe_download(url):
    filename = url.split('/')[-1]
    if not os.path.exists(filename):
        print("Downloading %s" % filename)
        urlretrieve(url, filename)

    # do something with file

url = 'http://....../bigfile.txt'
maybe_download(url)

