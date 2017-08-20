N = 10

(0..N).each { Post.create(title: Faker::Hipster.sentence, body: Faker::Hipster.paragraph(10, true, 5)) }
