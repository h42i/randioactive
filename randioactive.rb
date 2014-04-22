require 'sinatra'
require 'json'

def push_to_safecast
  # "I should push the CPM to Safecast."
  # http://blog.safecast.org/
end

def process_data(data)
  @json_data = JSON.parse(data)
  @timestamp = Time.now.to_i

  puts "[UPDATE] #{data}"

  # Checking which type of update
  if @json_data["type"] == "click"
    # Processing and saving a click (with special timestamp)
    @click = @json_data["data"]["click"]
    @c_click = {"click"=>@click}.to_json
    File.open("./data/click.json", "a+") {|f| f.puts(@c_click)}
  elsif @json_data["type"] == "cpm"
    # Processing and saving the clicks per minute (with timestamp)
    @cpm = @json_data["data"]["cpm"]
    @c_cpm = {"cpm"=>@cpm, "timestamp"=>@timestamp}.to_json
    File.open("./data/cpm.json", "a+") {|f| f.puts(@c_cpm)}
  elsif @json_data["type"] == "random"
    # "yo dawg, I herd you like a bitstream socket..."
    # @r_bit  = @json_data["module"]["random"]["bit"]
    # bit  = {"bit"=>@r_bit, "timestamp"=>@timestamp}
    
    # Processing and saving a random byte (with timestamp)
    @r_byte = @json_data["data"]["random"]["byte"]
    @c_byte = {"byte"=>@r_byte, "timestamp"=>@timestamp}
    File.open("./data/random.json", "a+") {|f| f.puts(@c_byte)}

    # Making sure the random bytes are hot and fresh
    if `wc -l ./data/random.json`.split.first.to_i < 20
      File.delete("./data.random.json")
    end
  else
    # Throws an error. Obviously.
    puts "[ERROR] Malformed update request."
  end
end

post '/api/update' do
  request.body.rewind
  process_data(request.body.read)
end

