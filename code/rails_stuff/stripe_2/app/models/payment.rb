class Payment < ActiveRecord::Base
  def process
    customer = Stripe::Customer.create email: email, card: card_token
    Stripe::Charge.create customer: customer.id,
      amount: 1468,
      currency: 'usd'
  end
end
