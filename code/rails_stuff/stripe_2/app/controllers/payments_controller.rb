class PaymentsController < ApplicationController
  def index
  end

  def new
    @payment = Payment.new
  end

  def create
    @payment = Payment.new email: params[:email], card_token: params[:token]
    @payment.process
    @payment.save

    redirect_to root_path
  end
end
