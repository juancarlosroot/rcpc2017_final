#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class mWiFi : public cSimpleModule
{
private:
    // Declare an event, events are a self-send notification to this module
    cMessage *event;
    // Package length
    char msgname[2358];
public:
    mWiFi();   // constructor
    virtual ~mWiFi(); // destructor
protected: // functions from cSimpleModule
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
// Set event to NULL
mWiFi::mWiFi()
{
    event = nullptr;
}
// Free memory
mWiFi::~mWiFi()
{
    delete event;
}
// Initialize variable event
// Set a static anonymous Message String
// Get parameter sendMsgOnInit
//
void mWiFi::initialize()
{
    event = new cMessage("event");
    sprintf(msgname, "WiFi Message ");
    if (par("sendMsgOnInit").boolValue() == true) {
        //send an event at 0.004 (simulation time)
        scheduleAt(0.004, event);
    }
}

// Used for manage arrival messages
void mWiFi::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, send message\n";
        // send message
        send(new cMessage(msgname), "gate$o", 0);
        // cancel event
        cancelEvent(event);
        // get simulation time and sum 0.004
        scheduleAt(simTime()+0.004, event);
    }
}
// Declare module (same as .ned file)
Define_Module(mWiFi);

