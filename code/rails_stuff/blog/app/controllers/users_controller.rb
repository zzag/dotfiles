class UsersController < ApplicationController
  def index
    users = User.all.page(params[:page]).per(params[:per_page])
    render json: PaginationSerializer.new(users, each_serializer: UserSerializer, root: 'users')
  end

  def show
    user = User.find(params[:id])
    render json: UserSerializer.new(user, root: 'user')
  end
end
