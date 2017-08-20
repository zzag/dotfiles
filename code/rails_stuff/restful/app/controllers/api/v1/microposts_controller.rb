class Api::V1::MicropostsController < Api::V1::BaseController
  def index
    microposts = Micropost.all
    render json: ActiveModel::ArraySerializer.new(microposts, each_serializer: Api::V1::MicropostSerializer, root: 'microposts')
  end

  def show
    micropost = Micropost.find(params[:id])
    render json: Api::V1::MicropostSerializer.new(micropost)
  end
end

