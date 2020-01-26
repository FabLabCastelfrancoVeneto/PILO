In questa cartella tutto il codice per ricreare il progetto di base.
Quando si fa riferimento a ESP8266, in questo profetto di fa riferimento ai moduli WEMOS D1 mini:
https://www.wemos.cc/en/latest/d1/d1_mini.html
ma dovrebbe andare bene qualsiasi altra board simile.
La comunicazione si basa su MQTT.

Nella cartella "esp8266_teddyBear" c'è il codice che risiede nel device principale, che può essere un cuscino, una coperta o un orsacchiotto.
Il codice gira su una ESP8266, montata su una breakout board in modo che sia possibile collegarci in modo semplice dispositivi di input.
La brekout board in questione è questa:
https://github.com/FabLabCastelfrancoVeneto/WemosGroveShield
I pulsanti sono creati con una base rigida (supporto pcb), sormontato da una spugna sottile e forata in più punti, e da un ulteriore 
livello fatto di tessuto conduttivo. Questo "panino" viene poi infilato in una tasca con delle infografiche, e collegato appunto alla board.

Nella cartella "esp8266_rgbSpot" c'è il codice fatto per un esp8266 a cui è collegato un neopixel ring; questo nodo funge da faretto.

Nella cartella "esp8266_ikeaLamp" c'è il codice di un nodo P2D (info qui: https://github.com/FabLabCastelfrancoVeneto/P2D). 

Nella cartella "nodered" si trova invece il flow per NodeRed, che serve a creare le interazioni tra input (bottoni soft) e gli output.


||

In this folder all the code needed to create a base setup.
When we say "esp8266" we mean the WEMOS D1 mini: https://www.wemos.cc/en/latest/d1/d1_mini.html 
but any other ESP8266 board should be ok.
The communication between nodes is based on MQTT.

In the "esp8266_teddyBear" folder, there's the code used on the main device, that could be a pillow, a teddybear, a blanket...
It runs on a ESP8266, mounted on a breaksout board in order to connect easliy the input devices
The used breakout board is this:
https://github.com/FabLabCastelfrancoVeneto/WemosGroveShield
Soft buttons are done usig a pcb board as base, then a slice of thin drilled sponge, and a last layer done of conductive fabric.
This sandwitch is fittend into a fabric pocket, with infographic decoration on it, and then connected to the board.

In the "esp8266_rgbSpot" folder there's the code for an esp8266 and a neopixel array. It is used as a light spot.

In the "esp8266_ikeaLamp" folder there's the code for a P2D node (here more info about it: https://github.com/FabLabCastelfrancoVeneto/P2D). 

In the "nodered" folder, there's a flow for NodeRed, that is used to create all the interactions between the input (soft button) and the outputs.

