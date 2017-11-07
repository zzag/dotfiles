# database_cleaner

## Installation

Gemfile:
```ruby
gem 'database_cleaner'
```

## Configuration(rspec)

Create file ```spec/support/database_cleaner.rb```:
```ruby
RSpec.configure do |config|

  config.before(:suite) do
    DatabaseCleaner.strategy = :transaction
    DatabaseCleaner.clean_with(:truncation)
  end

  config.around(:each) do |example|
    DatabaseCleaner.cleaning do
      example.run
    end
  end

end
```
