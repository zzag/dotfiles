require 'bcrypt'
require 'securerandom'

class User < ActiveRecord::Base
  before_save :hash_password
  before_create :generate_auth_token

  has_many :posts

  def authenticate(password)
    self.password == BCrypt::Engine.hash_secret(password, self.salt)
  end

  private

  def generate_auth_token
    self.auth_token = SecureRandom.uuid.gsub(/\-/, '')
  end

  def hash_password
    self.salt = BCrypt::Engine.generate_salt
    self.password = BCrypt::Engine.hash_secret(password, self.salt)
  end
end
