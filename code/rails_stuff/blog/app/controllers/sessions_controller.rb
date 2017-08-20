class SessionsController < ApplicationController
  def create
    credentials = user_credentials
    user = User.find_by(email: credentials[:email])

    if user && user.authenticate(credentials[:password])
      headers['Authorization'] = "Token token=#{user.auth_token}"
      render json: UserSerializer.new(user)
    else
      render json: { error: 'Invalid credentials' }, status: 400
    end
  end

  private

  def user_credentials
    params.require(:user).permit(:email, :password)
  end
end
