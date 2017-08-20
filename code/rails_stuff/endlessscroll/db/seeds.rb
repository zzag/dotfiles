(1..200).each do |i|
  Post.create(title: "Ololo-#{i}", body: "Noisy stuff")
end
