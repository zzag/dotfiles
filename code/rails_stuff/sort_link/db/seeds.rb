require 'faker'

N = 20

(1..N).each do
  Dude.create(first_name: Faker::Name.first_name, last_name: Faker::Name.last_name, country: Faker::Address.country, birthday: Faker::Date.backward(30000))
end
