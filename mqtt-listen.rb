require 'mqtt'

MQTT::Client.connect('atlas.hasi') do |c|
  c.get('#') do |topic, message|
    puts "#{topic}: #{message}"
  end
end
