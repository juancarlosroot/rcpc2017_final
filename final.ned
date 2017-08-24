simple mConverter
{
    parameters:
        bool sendMsgOnInit = default(false);
        int probability;
        @display("i=device/antennatower_vl");
    gates:
        inout gate[];
}

simple mWiFi
{
    parameters:
        @display("i=device/wifilaptop_vl");
        bool sendMsgOnInit = true;
    gates:
        inout gate[];
}

simple mZigBee
{
    parameters:
        bool sendMsgOnInit = default(false);
        @display("i=device/card_vl");
    gates:
        inout gate[];
}

network RedHet
{
    @display("bgb=747,177");
    submodules:
        wifi: mWiFi {
            @display("p=683,94");
        }
        zigbee: mZigBee {
            @display("p=91,94");
        }
        converter: mConverter {
            @display("p=377,94");
        }
    connections:
        wifi.gate++ <--> {  delay = 4ms; } <--> converter.gate++;
        zigbee.gate++ <--> {  delay = 1.71ms; } <--> converter.gate++;
}



