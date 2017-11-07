# faye + thin server

```ruby
require 'faye'

# available adapters:
#   * goliath
#   * rainbows
#   * thin
Faye::WebSocket.load_adapter('thin')
bayeux = Faye::RackAdapter.new(:mount => '/faye', :timeout => 25)
run bayeux
```
