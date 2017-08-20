class Api::V1::UserSerializer < Api::V1::BaseSerializer
  attributes :id, :email, :admin, :created_at, :updated_at

  has_many :microposts
end
