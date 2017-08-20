require 'securerandom'

class User < ActiveRecord::Base
  has_many :microposts

  before_create :set_auth_token

  def authenticate(password)
    self.password == password
  end

  def set_auth_token
    return if auth_token.present?
    self.auth_token = generated_auth_token
  end

  def generated_auth_token
    SecureRandom.uuid.gsub(/\-/, '')
  end
end
