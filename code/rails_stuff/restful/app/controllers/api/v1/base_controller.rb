module Api
  module V1
    class BaseController < ApplicationController
      protect_from_forgery with: :null_session
      before_action :destroy_session

      def destroy_session
        request.session_options[:skip] = true
      end

      def authenticate
        authenticate_or_request_with_http_token do |token, options|
          User.find_by(auth_token: token)
        end
      end
    end
  end
end
