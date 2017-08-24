#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class mZigBee : public cSimpleModule
{
private:
    // Declare an event, events are a self-send notification to this module
    cOutVector numSentVector;
    cMessage *event;
    char msgname[131];
public:
    mZigBee();
    virtual ~mZigBee();
protected:  // functions from cSimpleModule
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
// Set event to NULL
mZigBee::mZigBee()
{
    event = nullptr;
}
// Free memory
mZigBee::~mZigBee()
{
    delete event;
}
void mZigBee::initialize()
{
    event = new cMessage("event");
    sprintf(msgname, "ZigBee Message ");
    scheduleAt(0.00171, event);
}
void mZigBee::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, send message\n";
        send(new cMessage(msgname), "gate$o", 0);
        cancelEvent(event);
        scheduleAt(simTime()+0.00171, event);
    }
}
Define_Module(mZigBee);
