class ApplicationController < ActionController::Base
  protect_from_forgery with: :exception
  before_filter :track_user, if: :user_signed_in?

  def track_user
    Activity.track_object(current_user)
  end
end
