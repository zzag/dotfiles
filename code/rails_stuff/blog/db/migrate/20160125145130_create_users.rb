class CreateUsers < ActiveRecord::Migration
  def change
    create_table :users do |t|
      t.string :email
      t.string :salt
      t.string :password
      t.string :auth_token

      t.timestamps null: false
    end
    add_index :users, :email
    add_index :users, :auth_token
  end
end
