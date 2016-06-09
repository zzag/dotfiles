# carrierwave

## Gemfile

```ruby
gem 'carrierwave'
gem 'mini_magick'
```

## Generate uploader

```sh
$ rails generate uploader Name
```

## Edit ```application.rb```:

```ruby
require 'carrierwave/orm/activerecord'
```

## Model config

```sh
$ rails g migration add_attachment_to_models attachment:string
```

```ruby
class Model < ActiveRecord::Base
  mount_uploader :image, ImageUploader
end
```

```ruby
model.image.url
```
