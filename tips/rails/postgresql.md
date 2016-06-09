# PostgreSQL

## Dependencies:
```sh
$ sudo apt install libpq-dev
```

## Gemfile

```ruby
gem 'pg'
```

## Create user

```sh
$ sudo su - postgres
$ psql
$ create role username with createdb login password 'password';
```

## Configure

Edit ```config/database.yml```:
```yml
default: &default
  adapter: postgresql
  encoding: utf-8
  pool: 5
  host: host
  username: username
  password: password

development:
  <<: *default
  database: dbdev

test:
  <<: *default
  database: dbtest

production:
  <<: *default
  database: dbprod
```

```sh
$ rake db:setup
```
