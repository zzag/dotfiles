# factory_girl

## Installation

Gemfile:
```ruby
gem 'factory_girl'
```

## Create factories

Create factory in ```spec/factories``` folder:
```ruby
FactoryGirl.define do
  factory :post do
    title "post title"
    body "post body"
  end
end

FactoryGirl::find_definitions
FactoryGirl::create(:post) #=> ...
```

## With faker

Gemfile:
```ruby
[...]
gem 'faker'
[...]
```

```ruby
FactoryGirl.define do
  factory :post do
    title { Faker::Hipster.sentence(5) }
    body { Faker::Hipster.paragraph(10) }
  end
end
```
