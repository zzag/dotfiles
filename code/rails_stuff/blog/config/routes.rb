Rails.application.routes.draw do
  scope :api do
    resources :users, only: [:index, :show] do
      resources :posts, only: [:index, :show]
    end
    resources :sessions, only: [:create]
  end
  match "*all" => "application#cors_preflight_check", via: :options
end
