require 'json'

counter = 0

def post(data)
  p data
  `curl --data '#{data}' http://127.0.0.1:4567/api/update`
end

loop do
  @click = {"type"=>"click", "data"=>{"click"=>counter}}
  post(@click.to_json)

  counter += 5

  if (counter % 10) == 0
    @cpm = {"type"=>"cpm", "data"=>{"cpm"=>rand(0..100)}}
    post(@cpm.to_json)
  end

  sleep 5
end
