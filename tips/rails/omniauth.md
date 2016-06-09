# OmniAuth + Devise


## Gemfile

```ruby
gem 'devise'

gem 'omniauth-twitter'
gem 'omniauth-facebook'
gem 'omniauth-github'
gem 'omniauth-google'
```

## Edit ```config/initializers/devise.rb```:

Add provider(twitter):
```ruby
config.omniauth :twitter, ENV["KEY"], ENV["SECRET"]
```

## Initialize devise

```sh
$ rails g devise:install
$ rails g devise User
$ rake db:migrate
```

## Add provider and uid columns

```sh
$ rails g migration AddOmniauthToUsers provider uid
$ rake db:migrate
```

## Edit ```User``` model

Add ```:omniauthable``` to model and from_omniauth class method:
```ruby
class User < ActiveRecord::Base
  devise :database_authenticatable, :registerable, :omniauthable,
         :recoverable, :rememberable, :trackable, :validatable

  def self.from_omniauth(auth)
    where(provider: auth.provider, uid: auth.uid).first_or_create do |user|
      user.password = Devise.friendly_token[0,20]
      user.provider = auth.provider
      user.uid = auth.uid
      user.name = auth.info.name
    end
  end
end
```

## Add callbacks controller

Edit ```config/routes.rb```:
```ruby
Rails.application.routes.draw do
  devise_for :users, controllers: { omniauth_callbacks: "callbacks" }
  # ...
end
```

Create ```app/controllers/callbacks_controller.rb```:
```ruby
class CallbacksController < Devise::OmniauthCallbacksController
  def twitter
    auth = request.env["omniauth.auth"]
    user = User.from_omniauth(auth)
    sign_in_and_redirect user
  end
end
```
