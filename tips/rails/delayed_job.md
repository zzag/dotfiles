# delayed_job

## Installation

Gemfile:
```ruby
gem 'delayed_job_active_record'
```

Create the table for queuing jobs:
```sh
$ rails g delayed_job:active_record
```

Migrate database:
```sh
$ rake db:migrate
```

Run worker process:
```sh
$ rake jobs:work
```

Active Job:
```ruby
config.active_job.queue_adapter = :delayed_job
```

## Queuing jobs

Call ```.delay.method(params)``` on any object and it will be processed in the background:
```ruby
obj.delay.do_something(params)
```

If a method should always be run in the background:
```ruby
class User
  def activate
  end
  handle_asynchronously :activate
end

user = User.new
user.activate
```
