class PostsController < ApplicationController
  def index
    @posts = Post.all.page params[:page]
  end
end
