json.messages @messages do |message|
    json.id message.id
    json.message message.text
    json.(message, :created_at, :updated_at)
end
