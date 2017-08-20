class PostsController < ApplicationController
  before_action :set_user

  def index
    posts = @user.posts.page(params[:page]).per(params[:per_page])
    render json: PaginationSerializer.new(posts, each_serializer: PostSerializer, root: 'posts')
  end

  def show
    post = @user.posts.find(params[:id])
    render json: PostSerializer.new(post, root: 'post')
  end

  private

  def set_user
    @user = User.find(params[:user_id])
  end
end
