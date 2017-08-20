class PaymentsController < ApplicationController
  def index
    @payments = Payment.all
  end

  def new
    @payment = Payment.new
  end

  def create
    @payment = Payment.new email: params["stripeEmail"],
      card_token: params["stripeToken"]
    @payment.process
    @payment.save

    redirect_to root_path
  end
end
