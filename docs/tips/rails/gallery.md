# Gallery


```
<% @pictures.in_groups_of(2, false) do |group| %>
  <div class="row">
    <% group.each do |picture| %>
      <div class="col-md-6 col-sm-12">
        <%= image_tag picture.url %>
      </div>
    <% end %>
  </div>
<% end %>
```

