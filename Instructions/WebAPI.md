# Web API Usage

The WebUI communicates with the Boilermaker using HTTP API calls since it's a single page web user interface that dynamically updates using Ajax refresh. All settings except for the WiFi configuration can be configured using plain old HTTP GET requests, which means you can configure pretty much everything just by typing URLs into your web browser's address bar. WiFi configuration has to be done via serial connection because you can't use the WebAPI unless it's already connected to WiFi.

If you use **curl** in Linux or Mac OS to configure it, you have to tell it to use an old HTTP protocol. This is because there is limited code space in an ESP32 and modern HTTP protocols would have an absolutely huge code footprint. For example, if your Boilermaker has the IP address 192.168.1.150 and you wanted to read the current temperature in Celcius, you would use this command.

`curl -s --http0.9 http://192.168.1.150/get-tempc`

The `-s` option silences the network traffic progress that **curl** normally prints on the screen so all you get is the temperature value and the `--http0.9` tells **curl** what HTTP protocol to use. If you don't specify the 0.9 protocol, you will only get an empty output.
