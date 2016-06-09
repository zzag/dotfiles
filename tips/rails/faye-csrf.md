# faye. csrf

```ruby
# server side

class CsrfExt
  def incoming(message, request, callback)
    session_token = request.session['_csrf_token']
    message_token = message['ext'] && message['ext'].delete('csrfToken')

    unless session_token == message_token
      message['error'] = "401::Access denied"
    end

    callback.call(message)
  end
end


config.middleware.insert_after ActionDispatch::Session::CookieStore,
  Faye::RackAdapter,
  extensions: [CsrfExt.new],
  mount: '/faye',
  timeout: 25
```

```js
// client side

var client = new Faye.Client('/faye');

client.addExtension({
  outgoing: function (message, callback) {
    message.ext = message.ext || {};
    message.ext.csrfToken = $('meta[name=csrf-token]').attr('content');
    callback(message)
  }
});
```





