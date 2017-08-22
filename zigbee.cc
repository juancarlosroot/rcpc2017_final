#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class mZigBee : public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    cOutVector numSentVector;
    cMessage *event;
    char msgname[131];
public:
    mZigBee();
    virtual ~mZigBee();
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
mZigBee::mZigBee()
{
    event = nullptr;
}
mZigBee::~mZigBee()
{
    delete event;
}
void mZigBee::initialize()
{
    event = new cMessage("event");
    WATCH(numSent);
    numSentVector.setName("Sent Vector");
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

//    delete msg;
}
Define_Module(mZigBee);
