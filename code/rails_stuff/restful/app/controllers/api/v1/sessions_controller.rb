class Api::V1::SessionsController < Api::V1::BaseController
  def create
    params = create_params
    user = User.find_by(email: params[:email])
    if user && user.authenticate(params[:password])
      headers['Authorization'] = "Token token=#{user.auth_token}"
      render json: Api::V1::SessionSerializer.new(user)
    else
      payload = { 'msg' => 'invalid email or password' }
      render json: payload, status: 400
    end
  end

  private

  def create_params
    params.require(:user).permit(:email, :password)
  end
end
