# HTTPS

## Generate a Private Key

```sh
$ openssl genrsa -des3 -out server.key 1024
```

## Generate a CSR

```sh
$ openssl req -new -key server.key -out server.csr
```

## Remove Passphrase from Key

```sh
$ openssl rsa -in server.key -out servernopass.key
```

## Generating a Self-Signed Certificate

```sh
$ openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt
```

In one motion:
```sh
$ sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout server.key -out server.crt
```

## Configure Apache

```sh
$ sudo a2enmod ssl
```

```
SSLEngine on
SSLProtocol all -SSLv2
SSLCertificateFile    /etc/ssl/certs/server.crt
SSLCertificateKeyFile /etc/ssl/private/server.key
```

## Configure nginx

```
server {
  listen 443 ssl;
  server_name www.example.com;
  ssl_certificate     www.example.com.crt;
  ssl_certificate_key www.example.com.key;
}
```

