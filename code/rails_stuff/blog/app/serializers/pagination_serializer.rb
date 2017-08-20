class PaginationSerializer < ActiveModel::ArraySerializer
  def initialize(object, options={})
    meta_key = options[:meta_key] || :meta
    options[meta_key] ||= {}
    options[meta_key] = {
      current_page: object.current_page,
      next_page: object.next_page,
      prev_page: object.prev_page,
      per_page: object.size,
      total_pages: object.total_pages,
      total_count: object.total_count
    }
    super(object, options)
  end
end
