module PostsHelper
  def extract_links(html)
    html.gsub(/https?:\/\/[^\s<>]+/) do |url|
      "<a href=\"#{url}\">#{url}</a>"
    end
  end
end
