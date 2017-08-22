#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class mWiFi : public cSimpleModule
{
private:
    long numSent;
    long numReceived;
    int nPackages;
    cOutVector numSentVector;
    cMessage *event;
    void scheduleMessage();
    char msgname[2358];
public:
    mWiFi();   // constructor
    virtual ~mWiFi(); // destructor
protected:
    virtual void initialize() override; // @Override
    virtual void handleMessage(cMessage *msg) override;
};
mWiFi::mWiFi()
{
    event = nullptr;
}
mWiFi::~mWiFi()
{
        delete event;
}
void mWiFi::initialize()
{
    numSent = 0;
    numReceived = 0;
    nPackages = par("totalPackages");
    WATCH(numSent);
    WATCH(numReceived);
    numSentVector.setName("Sent Vector");
    event = new cMessage("event");

    sprintf(msgname, "WiFi Message ");
    if (par("sendMsgOnInit").boolValue() == true) {
        scheduleAt(0.004, event);
    }
}
void mWiFi::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, send message\n";
        send(new cMessage(msgname), "gate$o", 0);
        cancelEvent(event);
        scheduleAt(simTime()+0.004, event);
    }

//    delete msg;
}

void mWiFi::scheduleMessage()
{
    simtime_t tmp = par("delayTime");
    EV << "(WiFi) normal distribution value : "<< tmp << "\n";
    scheduleAt(simTime()+tmp, new cMessage("WiFi Message"));
}
Define_Module(mWiFi);

