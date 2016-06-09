# Passenger

```
server {
  listen 80;
  server_name example.com;
  passenger_enabled on;
  passenger_app_env production;
  root /var/www/appname/public;
}
```
