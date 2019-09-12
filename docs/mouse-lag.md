# Change Bluetooth mouse connection parameters

```sh
sudo vim /var/lib/bluetooth/xx\:xx\:xx\:xx\:xx\:xx/yy\:yy\:yy\:yy\:yy\:yy/info
```

Paste the following section

```
[ConnectionParameters]
MinInterval=6
MaxInterval=9
Latency=44
Timeout=216
```
