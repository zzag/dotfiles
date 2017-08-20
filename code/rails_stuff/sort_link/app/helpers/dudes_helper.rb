module DudesHelper
  def sort_link(by, name)
    direction = by.to_s == sort_column && sort_direction == "asc" ? "desc" : "asc"
    active_link = by.to_s == sort_column ? direction : nil
    klass = "sort_link #{active_link}"
    link_to name, { sort: by, order: direction }, { class: klass }
  end
end
