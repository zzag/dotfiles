# faye + custom extensions


```ruby
require 'faye'

class Auth
  def incoming(message, callback)
    auth_token = message['ext']['auth_token']

    if auth_token != AUTH_TOKEN
      message['error'] = "401"
    end

    callback.call(message)
  end

  def outgoing(message, callback)
    if message['ext'] && message['ext']['auth_token']
      message['ext'].delete('auth_token')
    end
    callback.call(message)
  end
end


bayeux = Faye::RackAdapter.new(:mount => '/faye', :timeout => 25, :extensions => [Auth.new])
run bayeux
```

```ruby
config.middleware.insert_after ActionDispatch::Session::CookieStore,
                               Faye::RackAdapter,
                               extensions: [CsrfProtection.new],
                               mount: '/faye',
                               timeout: 25
```
