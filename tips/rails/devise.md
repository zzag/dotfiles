# devise

## Custom fields

```ruby
class ApplicationController < ActionController::Base
  before_action :configure_permitted_parameters, if:  :devise_controller?

  private
  def configure_permitted_parameters
    devise_parameter_sanitizer.for(:sign_up) << :name
    devise_parameter_sanitizer.for(:sign_in) << :name
    devise_parameter_sanitizer.for(:account_update) << :name
  end
end
```
