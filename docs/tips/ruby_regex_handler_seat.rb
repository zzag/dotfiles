class RegexHandlerSeat
  def initialize
    @handlers = {}
  end

  def handle(pattern, &block)
    @handlers[pattern] = block
  end

  def run(input)
    @handlers.each do |pattern, block|
      if match = input.match(pattern)
        break block.call(match)
      end
    end
  end

  def self.run(input, &block)
    seat = RegexHandlerSeat.new
    seat.instance_eval(&block)
    seat.run(input)
  end
end


# 1
handlers = RegexHandlerSeat.new
handlers.handle(/hello/i) { puts "Handle hello" }
handlers.handle(/My name is (\w+)/) { |match| puts "Hello, #{match[1]}" }
handlers.handle(/Hi, (?<name>\w+)/) { |match| puts "#{match['name']}: what's up?" }

handlers.run "hello world"
handlers.run "My name is daniel"
handlers.run "Hi, daniel"

# 2
RegexHandlerSeat.run("hello, world") do
  handle(/hello/i) { puts "Handle hello" }
  handle(/My name is (\w+)/) { |match| puts "Hello, #{match[1]}" }
  handle(/Hi, (?<name>\w+)/) { |match| puts "#{match['name']}: what's up?" }
end

