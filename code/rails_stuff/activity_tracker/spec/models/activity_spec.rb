require 'rails_helper'

RSpec.describe Activity do
  context "#key" do
    let(:class_name) { double(:name => "User") }
    let(:user) { double("User", { :id => 1, :class => class_name }) }
    let(:now) { Time.now }
    let(:activity_key) { "user:1" }
    let(:online_key) { "online:#{now.strftime("%M")}" }

    it "should build valid activity key" do
      expect(Activity.activity_key(user)).to eq activity_key
    end

    it "should build valid online key" do
      expect(Activity.online_key(now)).to eq online_key
    end
  end
end
