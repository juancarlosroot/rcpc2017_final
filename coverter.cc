#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;
class mConverter : public cSimpleModule
{
private:
    long numSent[2] = {0, 0};
    long numReceived[2] = {0, 0};
    long numLost[2] = {0, 0};
    simtime_t startTime, endTime;
    cOutVector numSentVectorW;
    cOutVector numSentVectorZ;
    cOutVector numRecvVectorW;
    cOutVector numRecvVectorZ;
    cOutVector numLostVectorW;
    cOutVector numLostVectorZ;
    cMessage *tictocMsg;
    cQueue mQueue;
    void fnPackageLost(int receivedFrom, int sendTo, cMessage *msg);
    void fnRecordSent(int i);
    void fnRecordRecv(int i);
    void fnRecordLost(int i);
public:
    mConverter();
    virtual ~mConverter();
protected:
    virtual void refreshDisplay() const override;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};
mConverter::mConverter()
{
    tictocMsg = nullptr;
    mQueue = nullptr;
}
mConverter::~mConverter()
{
    delete tictocMsg;
}
void mConverter::initialize()
{
    startTime = simTime();
    mQueue = nullptr;
    WATCH(numSent);
    WATCH(numReceived);
    WATCH(numLost);
    numSentVectorW.setName("(WiFi) Sent Vector");
    numSentVectorZ.setName("(ZigBee) Sent Vector");
    numRecvVectorW.setName("(WiFi) Received Vector");
    numRecvVectorZ.setName("(ZigBee) Received Vector");
    numLostVectorW.setName("(WiFi) Lost Vector");
    numLostVectorZ.setName("(ZigBee) Lost Vector");
}
void mConverter::handleMessage(cMessage *msg)
{
    // numSent[0] wifi
    // numSent[1] zigb
    if (uniform(0, 1) < 0.05) {
        EV << "\"Losing\" message " << msg << endl;
//        numLost[msg->getArrivalGate()->getIndex()]++;
//        fnRecordLost(msg->getArrivalGate()->getIndex());
        bubble("message lost");
    }
    else {
        numSent[msg->getArrivalGate()->getIndex()]++;
        fnRecordSent(msg->getArrivalGate()->getIndex());

        EV << "Forwarding message " << msg << " on gate[" << msg->getArrivalGate()->getIndex() << "]\n";

        if (msg->getArrivalGate()->getIndex() == 0) // Llega de Wifi
        {
            for(int i = 0 ; i < 18; i++)
            {
                char msgname[131];
                sprintf(msgname, "id - %d", i);
                send(new cMessage(msgname), "gate$o", 1);
            }
        }
        else if (msg->getArrivalGate()->getIndex() == 1)// Llega de ZB
        {
            char msgname[2358];
            sprintf(msgname, "To Wifi ");
            send(new cMessage(msgname), "gate$o", 0);
        }
    }
    delete msg;
}

void mConverter::fnPackageLost(int receivedFrom, int sendTo, cMessage *msg)
{
    if (uniform(0, 1) < 0.5) {
        EV << "\"Losing\" message " << msg << endl;
        bubble("message lost");
//        numLost[receivedFrom]++;
//        fnRecordLost(receivedFrom);
        delete msg;
    }
    else {
        tictocMsg = msg;
        numReceived[sendTo]++;
        fnRecordRecv(sendTo);
        send(tictocMsg, "gate$o", sendTo);
    }
}

void mConverter::fnRecordSent(int i)
{
    if(i == 0)
    {
        numSentVectorW.record(numSent[i]);
    }
    else
    {
        numSentVectorZ.record(numSent[i]);
    }
}
void mConverter::fnRecordRecv(int i)
{
    if(i == 0)
    {
        numRecvVectorW.record(numReceived[i]);
    }
    else
    {
        numRecvVectorZ.record(numReceived[i]);
    }
}
void mConverter::fnRecordLost(int i)
{
    if(i == 0)
    {
        numLostVectorW.record(numLost[i]);
    }
    else
    {
        numLostVectorZ.record(numLost[i]);
    }
}
void mConverter::finish()
{
    endTime = simTime() - startTime;
    EV << "Total TIME : " << endTime << endl;
};

void mConverter::refreshDisplay() const
{
    char buf1[100];
    sprintf(
            buf1,
            "WiFi Received: %ld Sent: %ld Lost: %ld \n ZigB Received: %ld Sent: %ld Lost: %ld",
            numReceived[0], numSent[0], numLost[0],
            numReceived[1], numSent[1], numLost[1]
    );
    getDisplayString().setTagArg("t", 0, buf1);
}

Define_Module(mConverter);
