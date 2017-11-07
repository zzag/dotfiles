# mongoid

## Installation

Gemfile:
```ruby
gem 'mongoid'
```

Generate config file:
```sh
$ rails g mongoid:config
$ vim config/mongoid.conf
```

## Define 'models'

```ruby
class User
  include Mongoid::Document
  include Mongoid::Timestamps::Created
  include Mongoid::Timestamps::Updated

  has_many :posts # or embeds_many :posts
  
  field :email, type: String
  field :password, type: String
end
```

```ruby
class Post
  include Mongoid::Document
  include Mongoid::Timestamps::Created
  include Mongoid::Timestamps::Updated
  
  belongs_to :user # or embedded_in :user

  field :title, type: String
  field :body, type: String
end
```
