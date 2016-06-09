# Puma server

## Gemfile

```ruby
gem 'puma'
```

## Config

Create configuration file ```config/puma.rb```
```ruby
# app dir
app_dir = File.expand_path("../..", __FILE__)
puma_dir = "#{app_dir}/puma"
directory app_dir

# puma config
environment ENV['RACK_ENV'] || 'development'
workers 2
worker_timeout 60
threads_count = Integer(ENV['MAX_THREADS'] || 5)
threads threads_count, threads_count
port ENV['PORT'] || 3000
daemonize false

# logs
stdout_redirect "#{puma_dir}/log/stdout", "#{puma_dir}/log/stderr"

# pid file
pidfile "#{puma_dir}/pids/puma.pid"
```

```sh
$ mkdir -p puma/log puma/pids
```

## Run

```sh
$ bundle exec puma --config config/puma.rb
```

## Log

To show logs on $stdout comment these lines in ```config/puma.rb```:
```ruby
stdout_redirect "#{puma_dir}/log/stdout", "#{puma_dir}/log/stderr"
```
