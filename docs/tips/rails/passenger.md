# Passenger

## Installation

```sh
$ gem install passenger
```

## Apache 2

Install module:
```sh
$ sudo apt install libcurl4-openssl-dev apache2-dev libapr1-dev libaprutil1-dev
$ passenger-install-apache2-module
```

Append to ```/etc/apache2/apache2.conf``` output of ```passenger-install-apache2-module```:
```
LoadModule passenger_module path-to-ruby/lib/ruby/gems/2.3.0/gems/passenger-5.0.23/buildout/apache2/mod_passenger.so
   <IfModule mod_passenger.c>
     PassengerRoot path-to-ruby/lib/ruby/gems/2.3.0/gems/passenger-5.0.23
     PassengerDefaultRuby path-to-ruby/bin/ruby
   </IfModule>
```

```sh
$ sudo a2enmod rewrite
$ sudo service apache2 restart
```

Create app:
```sh
$ cd /var/www/html
$ rails new appname
```

Configuration file ```/etc/apache2/sites-available/appname.conf```:
```
<VirtualHost *:80>
 RailsEnv development
 ServerName www.somehostname.com
 DocumentRoot /var/www/html/appname/public
 <Directory /var/www/html/appname/public>
  AllowOverride all
  Options -MultiViews
 </Directory>
</VirtualHost>
```

Optional edit ```/etc/hosts```:
```
127.0.0.1  www.somehostname.com
```

Enable site and restart server:
```sh
$ sudo a2ensite appname
$ sudo service apache2 restart
```


## Nginx

Build new nginx(/opt/nginx):
```sh
$ passenger-install-nginx-module
```

Config passenger:
```
http {
    ...
    passenger_root path-to-ruby/lib/ruby/gems/2.3.0/gems/passenger-5.0.23;
    passenger_ruby path-to-ruby/bin/ruby;
    ...
}
```

Create app:
```sh
$ cd /var/www
$ rails new appname
$ # ...
```


Set ```secret_key_base``` in ```config/secrets.yml```:
```sh
$ rake secret # copy output
```

Edit configuration file ```/opt/nginx/conf/nginx.conf```:
```
server {
  listen 80;
  server_name example.com;
  passenger_enabled on;
  passenger_app_env production;
  root /var/www/appname/public;
}
```
