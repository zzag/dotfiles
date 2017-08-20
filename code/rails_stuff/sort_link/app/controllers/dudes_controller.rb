class DudesController < ApplicationController
  def index
    @dudes = Dude.all

    if sort?
      @dudes = Dude.all.order(sort_params)
    end
  end

  def sort?
    sort_column.present?
  end

  def sort_params
    "#{sort_column} #{sort_direction}"
  end

  def sort_column
    params[:sort] if Dude.column_names.include?(params[:sort])
  end

  def sort_direction
    %w(asc desc).include?(params[:order]) ? params[:order] : "asc"
  end

  helper_method :sort_column, :sort_direction
end
