## Usage

```application_controller.rb```:
```ruby
include ActionController::HttpAuthentication::Basic::ControllerMethods
include ActionController::HttpAuthentication::Token::ControllerMethods
```

```sh
$ curl -X POST --data 'user[email]=jack@example.com&user[password]=12345678'  http://localhost:3000/api/v1/sessions -v
$ curl -H "Authorization: Token token=de1fe46dc45845db8005a5043647028b" http://localhost:3000/api/v1/users/
```

With Basic strategy:
```ruby
def token
  authenticate_with_http_basic do |email, password|
    user = User.find_by(email: email)
    if user && user.password == password
      render json: { token: user.auth_token }
    else
      render json: { error: "Incorrect credentials" }, status: 401
    end
  end
end
```

Token strategy:
```ruy
before_filter :authenticate_user_from_token, except: [:token]

def authenticate_user_from_token
  unless authenticate_with_http_token { |token, options| User.find_by(auth_token: token) }
    render json: { error: 'Bad token' }, status: 400
  end
end
```



```ruby
authenticate_or_request_with_http_token do |token, options|
end
```
