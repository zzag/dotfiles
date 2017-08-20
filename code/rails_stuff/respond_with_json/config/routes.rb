Rails.application.routes.draw do
    resources :messages, defaults: { format: 'json' }
    root to: "messages#index", defaults: { format: 'json' }
end
