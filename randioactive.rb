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
    # Processing and saving a click
    @click = @json_data["data"]["click"]
    @c_click = {"click"=>@click}.to_json
    File.open("./data/click.json", "a+") {|f| f.puts(@c_click)}
  elsif @json_data["type"] == "cpm"
    # 
    @cpm = @json_data["data"]["cpm"]
    @c_cpm = {"cpm"=>@cpm, "timestamp"=>@timestamp}.to_json
    File.open("./data/cpm.json", "a+") {|f| f.puts(@c_cpm)}
  elsif @json_data["type"] == "random"
    
    # "yo dawg, I herd you like a bitstream socket..."
    # @r_bit  = @json_data["module"]["random"]["bit"]
    # bit  = {"bit"=>@r_bit, "timestamp"=>@timestamp}
    
    @r_byte = @json_data["data"]["random"]["byte"]
    @c_byte = {"byte"=>@r_byte, "timestamp"=>@timestamp}

  else
    puts "[ERROR] Malformed update request."
  end

end

post '/api/update' do
  request.body.rewind
  process_data(request.body.read)
end

