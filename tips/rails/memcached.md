# memcached 

## Installation

Gemfile:
```ruby
gem 'dalli'
```

## Configuration

Edit environment config:
```ruby
config.cache_store = :dalli_store

# or
config.cache_store = :dalli_store, 'cache.example.com', { namespace: APP_NAME, expires_in: 1.min, compress: true }
```

## Usage

```ruby
Rails.cache.read(:foo)
Rails.cache.write(:foo, 123)
Rails.cache.fetch(:foo) do 
  # query
end
Rails.cache.delete(:foo)
```
