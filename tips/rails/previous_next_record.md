# Previous Next Record


```ruby

class Item < ActiveRecord::Base
  def previous
    Item.all.where('id < ?', id).last
  end

  def next
    Item.all.where('id > ?', id).first
  end
end

```
