//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple Txc7
{
    parameters:
        volatile double delayTime @unit(s);   // delay before sending back message
        bool sendMsgOnInit = default(false);
        @display("i=block/routing");
	gates:
        input in[]; 
        output out[];
}

simple Converter extends Txc7
{
    @display("i=device/antennatower_vl");
	@display("p=50,79;r=150");
}

simple WiFi extends Txc7
{
    parameters:
        @display("i=device/wifilaptop_vl");
        sendMsgOnInit = true;   // Tic modules should send a message on init
}

simple ZigBee extends Txc7
{
    parameters:
        @display("i=device/card_vl");  // Toc modules should NOT send a message on init
}

network Tictoc1
{
    submodules:
        wifi: WiFi;  // the limit parameter is still unbound here. We will get it from the ini file
        zigbee: ZigBee;
        converter: Converter;
    connections:
        wifi.out++ --> {  delay = 100ms; } --> converter.in++;
        wifi.in++ <-- {  delay = 100ms; } <-- converter.out++;
        
        zigbee.out++ --> {  delay = 100ms; } --> converter.in++;
        zigbee.in++ <-- {  delay = 100ms; } <-- converter.out++;
}



