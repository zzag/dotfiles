class CreateMicroposts < ActiveRecord::Migration
  def change
    create_table :microposts do |t|
      t.integer :user_id
      t.string :title
      t.text :body

      t.timestamps null: false
    end
    add_index :microposts, :user_id
  end
end
