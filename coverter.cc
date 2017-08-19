#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class mConverter : public cSimpleModule
{
  private:
    cMessage *event;
    cMessage *tictocMsg;
  public:
    mConverter();
    virtual ~mConverter();
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};
mConverter::mConverter()
{
    event = tictocMsg = nullptr;
}
mConverter::~mConverter()
{
    cancelAndDelete(event);
    delete tictocMsg;
}
void mConverter::initialize()
{
    event = new cMessage("event");
    tictocMsg = nullptr;
    if (strcmp("wifi", getName()) == 0) {
        EV << "Scheduling first send to t=5.0s\n";
        scheduleAt(5.0, event);
        tictocMsg = new cMessage("tictocMsg");
    }
}
void mConverter::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, sending back message\n";
        forwardMessage(tictocMsg);
        tictocMsg = nullptr;
    }
    else {
        // "Lose" the message with 0.1 probability:
        if (uniform(0, 1) < 0.1) {
            EV << "\"Losing\" message\n";
            delete msg;
        }
        else {
            // The "delayTime" module parameter can be set to values like
            // "exponential(5)" (tictoc7.ned, omnetpp.ini), and then here
            // we'll get a different delay every time.
            simtime_t delay = par("delayTime");
            EV << "Message arrived, starting to wait " << delay << " secs...\n";
            tictocMsg = msg;
            scheduleAt(simTime()+0.004, event);
        }
    }
}

void mConverter::forwardMessage(cMessage *msg)
{
    // In this example, we just pick a random gate to send it on.
    // We draw a random number between 0 and the size of gate `out[]'.
    int n = gateSize("out");
    int k = intuniform(0, n-1);

    EV << "Forwarding message " << msg->getArrivalGate() << "]\n";
    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}
Define_Module(mConverter);
