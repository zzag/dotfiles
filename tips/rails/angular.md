# AngularJS


### Create project

```sh
$ rails new appname --skip-javascript
```

### Create javascripts folder

```sh
$ mkdir -p app/assets/javascripts
```


### Create application.js and app.js

applications.js:
```js
//= require_tree .
```

app.js:
```js
```

### Include application.js in view

Append to head section in ```app/views/layouts/application.html.erb```:
```
<%= javascript_include_tag 'application' %>
```

### Rails routing

Create action in ```ApplicationController```:
```ruby
def angular
  render 'layouts/application'
end
```

Add a route to ```config/routes.rb```:
```ruby
root to: 'application#angular'
```

### Install rest dependencies(Bower)

Create ```.bowerrc```:
```
{
  "directory":"vendor/assets/bower_components"
}
```

Install some dependencies:
```sh
$ bower install angular angular-ui-router bootstrap --save
```

Edit ```app/assets/javascripts/application.js```:
```js
//= require angular
//= require angular-ui-router
//= require_tree .
```

Edit ```app/assets/stylesheets/application.css```:
```
*= require bootstrap/dist/css/bootstrap
*= require_tree .
```

### Angular templates

Edit ```Gemfile```:
```ruby
gem 'angular-rails-templates'
```

Append ```angular-rails-templates``` after ```angular``` in ```app/assets/javascripts/application.js```:
```js
//= require angular-rails-templates
```

Inject ```templates``` module to app module ```app/assets/javascripts/app.js```:
```js
var app = angular.module('app', ['templates']);

// ...
```


### Project structure

```
app
| - assets
    |- javascripts
       |- application.js
       |- app.js
       |- template1
          |- template.html
       |- template2
          |- templ.html.erb
```

```templateUrl``` :
```js
{
  templateUrl: 'template2/templ.html'
}
```


### Rails JSON response(without jsbuilder)

Suppose we have two models ```Post``` and ```Comment```:
```ruby
class Comment < ActiveRecord::Base
end

class Post < ActiveRecord::Base
  has_many :comments

  def as_json(options={})
    super(options.merge(include: :comments))
  end
end
```

Edit ```ApplicationController```:
```ruby
respond_to :json
```

#### Sample controllers

```ruby
class PostsController < ApplicationController
  def index
    respond_with Post.all
  end

  def create
    respond_with Post.create(post_params)
  end

  def show
    respond_with Post.find(params[:id])
  end

  private
  def post_params
    params.require(:post).permit(:link, :title)
  end
end
```

```ruby
class CommentsController < ApplicationController
  def create
    post = Post.find(params[:post_id])
    comment = post.comments.create(comment_params)
    respond_with post, comment
  end

  def do_smth
    post = Post.find(params[:post_id])
    comment = post.comments.find(params[:id])
    comment.do_smth(comment_params)

    respond_with post, comment
  end

  private
  def comment_params
    params.require(:comment).permit(:body)
  end
end
```
