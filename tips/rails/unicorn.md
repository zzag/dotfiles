# Unicorn server


## Gemfile

```ruby
gem 'unicorn'
```

## Config

Create configuration file ```config/unicorn.rb```

```ruby
# set path
app_dir = File.expand_path("../..", __FILE__)
unicorn_dir = "#{app_dir}/unicorn"
working_directory app_dir


# set options
worker_processes 1
preload_app true
timeout 30

# socket
listen "#{unicorn_dir}/sockets/unicorn.sock", :backlog => 64

# logs
stderr_path "#{unicorn_dir}/log/unicorn.stderr.log"
stdout_path "#{unicorn_dir}/log/unicorn.stdout.log"

# master PID location
pid "#{unicorn_dir}/pids/unicorn.pid"

```

```sh
$ mkdir -p unicorn/pids unicorn/log unicorn/sockets
```

## Run

```sh
$ bundle exec unicorn --port 3000 --config-file config/unicorn.rb
```

## Loging

To show logs on $stdout comment these lines in ```config/unicorn.rb```:
```ruby
stderr_path "#{unicorn_dir}/log/unicorn.stderr.log"
stdout_path "#{unicorn_dir}/log/unicorn.stdout.log"
```
