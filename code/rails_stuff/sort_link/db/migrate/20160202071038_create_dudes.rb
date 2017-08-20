class CreateDudes < ActiveRecord::Migration
  def change
    create_table :dudes do |t|
      t.string :first_name
      t.string :last_name
      t.string :country
      t.datetime :birthday

      t.timestamps null: false
    end
  end
end
