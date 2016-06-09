# Search

## sunspot

### Gemfile

Edit Gemfile:
```ruby
gem 'sunspot_rails'
gem 'sunspot_solr'
```

### Configure

```sh
$ rails generate sunspot_rails:install
```


### Run Solr server

Background:
```sh
$ bundle exec rake sunspot:solr:start
```

Foreground:
```sh
$ bundle exec rake sunspot:solr:run
```

### Configure model

```ruby
class Post < ActiveRecord::Base
  searchable do
    text :title, :body
    integer :author_id
    time    :published_at
  end
end
```

### Search

```ruby
search = Post.search { fulltext 'random text' }

search = Post.search do
  fulltext 'random text'

  with :author_id, 123
  with(:published_at).less_than Time.now
  order_by :published_at, :desc
end

total = search.total
results = search.results
```

### Reindex

```sh
$ bundle exec rake sunspot:reindex
```

or

```ruby
Post.reindex
```
