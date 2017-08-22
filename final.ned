simple mConverter
{
    parameters:
        bool sendMsgOnInit = default(false);
        @display("i=device/antennatower_vl");
        @display("p=50,79;r=150");
    gates:
        inout gate[];
}

simple mWiFi
{
    parameters:
        int totalPackages = default(5);
        volatile double delayTime @unit(s);
        @display("i=device/wifilaptop_vl");
        bool sendMsgOnInit = true;   // Tic modules should send a message on init
    gates:
        inout gate[];
}

simple mZigBee
{
    parameters:
        bool sendMsgOnInit = default(false);
        @display("i=device/card_vl");  // Toc modules should NOT send a message on init
    gates:
        inout gate[];
}

network Tictoc1
{
    submodules:
        wifi: mWiFi;
        zigbee: mZigBee;
        converter: mConverter;
    connections:
        wifi.gate++ <--> {  delay = 4ms; } <--> converter.gate++;
        zigbee.gate++ <--> {  delay = 1.71ms; } <--> converter.gate++;
}



