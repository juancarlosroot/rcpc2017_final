//simple mModule
//{
//    parameters:
//        volatile double delayTime @unit(s);   // delay before sending back message
//        bool sendMsgOnInit = default(false);
//        @display("i=block/routing");
//	gates:
//        input in[]; 
//        output out[];
//}

simple mConverter
{    parameters:
        volatile double delayTime @unit(s);   // delay before sending back message
        bool sendMsgOnInit = default(false);
        @display("i=device/antennatower_vl");
    	@display("p=50,79;r=150");
    gates:
        input in[]; 
        output out[];
}

simple mWiFi
{
    parameters:
        volatile double delayTime @unit(s);
        @display("i=device/wifilaptop_vl");
        bool sendMsgOnInit = true;   // Tic modules should send a message on init
	gates:
        input in[]; 
        output out[];
}

simple mZigBee
{
    parameters:
        volatile double delayTime @unit(s);   // delay before sending back message
        bool sendMsgOnInit = default(false);
        @display("i=device/card_vl");  // Toc modules should NOT send a message on init
	gates:
        input in[]; 
        output out[];
}

network Tictoc1
{
    submodules:
        wifi: mWiFi;  // the limit parameter is still unbound here. We will get it from the ini file
        zigbee: mZigBee;
        converter: mConverter;
    connections:
        wifi.out++ --> {  delay = 100ms; } --> converter.in++;
        wifi.in++ <-- {  delay = 100ms; } <-- converter.out++;
        
        zigbee.out++ --> {  delay = 100ms; } --> converter.in++;
        zigbee.in++ <-- {  delay = 100ms; } <-- converter.out++;
}



