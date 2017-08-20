Rails.application.routes.draw do
  resources :dudes
  root to: 'dudes#index'
end
