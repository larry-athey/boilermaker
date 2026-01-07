<img width="1024" src="https://github.com/user-attachments/assets/8f01c76d-e4c6-4d8b-bb70-96b255715c0f"><br>

The instructions on this page are for my original prototype, it ain't pretty, but I still have one running my electric smoker. The units that I sell on my website are based on the [Firefly PCB](https://github.com/larry-athey/boilermaker/tree/main/Diagrams/Firefly-PCB). The prototype model only requires the [Boilermaker-PSU-and-Interface](https://github.com/larry-athey/boilermaker/blob/main/Diagrams/Schematic_Boilermaker-PSU-and-Interface.pdf) to be wired up on a prototype PCB and connected to an ESP32.

<img width="1024" src="./PSU-and-Interface.jpg"><br>

As you can see, there's a whole lot of nothing to it. 4 wires come from it for the SSR + _(yellow)_, SSR - _(green)_, and two for the AC power input to the power supply _(blue and white)_. The cooling fan connects to the screw terminals _(+ on the left, - on the right)_. The 4-pin header is for the ESP32 for _(left to right)_ the fan trigger, SSR trigger, +5 volts, and ground. This only takes about an hour to assemble.

<img width="1024" src="./ESP32-Home.jpg"><br>

As you can see in the **ESP32-Home.jpg** photo in the above, the ESP32 simply rests in the recessed cavity in the case cover. Another cover goes over this _(the **BM-ESP32-Cover.stl** model)_ to keep the ESP32 in place. This cavity is also big enough to hold the LilyGo ABS shell for their standard non-touch T-Display-S3 module. I've used a 38 pin WROOM-DA in this photo with an external antenna.

<img width="1024" src="./Case-Cover-Assembled.jpg"><br>

In the **Case-Cover-Assembled.jpg** photo shown above, you can see the placement of the PSU and interface board, the MAX-6675 thermocouple amplifier, and the ribbon cables used to connect everything to the ESP32 pins. The external antenna is located between the rows of the ESP32 pins. If you bundle up the ribbon cables with zip ties, this will keep everything out of the way of the heatsink.

<img width="1024" src="./Case-Bottom-Assembled.jpg"><br>

In the **Case-Bottom-Assembled.jpg** photo shown above, you can see the SSR and heatink as well as a screw terminal bus bar installed in the case bottom. The trigger wires to the SSR are connected and you would connect your AC power input/output lines at this point. Connect the ESP32 PSU input lines to the AC input side of the SSR and the bus bar. Honestly, it's totally up to you if you want to switch the AC hot or neutral line with the SSR, it honestly makes no difference. Connect the fan & thermocouple and you're done.

<img width="1024" src="./Assembled-Boilermaker.jpg"><br>

And, as you can see in the **Assembled-Boilermaker.jpg** photo above, you have an assembled Boilermaker that's ready to go. Your choice of SSR will vary, depending on the load that you intend to run. Refer to the last section in the [Usage Tips](https://github.com/larry-athey/boilermaker/blob/main/Instructions/Usage-Tips.md) document for more information. While the amperage rating is important, the size of the heatink and having a cooling fan are equally important. You can very easily blow an SSR-100DA with a mere 1500 watt load if you don't keep it cool enough.
