# Redis cache

## Gemfile

```ruby
gem 'redis'
gem 'redis-namespace'
gem 'redis-rails'
```

## Initialize

```ruby
# config/application.rb
config.cache_store = :redis_store, "redis://localhost:6379/0/cache", { expires_in: 90.minutes }

# config/initializers/redis.rb
$redis = Redis::Namespace.new("ns", redis: Redis.new)
```

## Usage

```ruby
module ItemHelper
  def fetch_items
    items = $redis.get "items"
    if items.nil?
      items = Item.all.to_json
      $redis.set "items", items
      $redis.expire "items", 1.hour.to_i
    end
    JSON.load items
  end
end
```
