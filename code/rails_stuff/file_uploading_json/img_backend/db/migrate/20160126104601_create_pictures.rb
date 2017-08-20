class CreatePictures < ActiveRecord::Migration
  def change
    create_table :pictures do |t|
      t.string :upload

      t.timestamps null: false
    end
  end
end
