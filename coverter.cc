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
    int probability;
    float fProbability;
    simtime_t startTime, endTime;
    cOutVector numSentVectorW;
    cOutVector numSentVectorZ;
//    cOutVector numRecvVectorW;
//    cOutVector numRecvVectorZ;
//    cOutVector numLostVectorW;
//    cOutVector numLostVectorZ;
    void fnRecordSent(int i);
//    void fnRecordRecv(int i);
//    void fnRecordLost(int i);
public:
    mConverter();// Constructor
    virtual ~mConverter();// Destructor
protected:  // functions from cSimpleModule
    virtual void refreshDisplay() const override;
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};
mConverter::mConverter()
{
}
mConverter::~mConverter()
{
}
void mConverter::initialize()
{
    // obtenemos parámetros de .ned(si los hay)
    probability = par("probability");
    // probabilidad en decimales
    fProbability = probability * 0.01;
    EV << "fProbability " << fProbability << endl;

    // guardamos el tiempo de inicio
    startTime = simTime();
    // colocamos observadores a nuestros arreglos
    WATCH(numSent);
    WATCH(numReceived);
    WATCH(numLost);
    // colocamos labels a nuestras gráficas
    numSentVectorW.setName("(WiFi) Sent Vector");
    numSentVectorZ.setName("(ZigBee) Sent Vector");
//    numRecvVectorW.setName("(WiFi) Received Vector");
//    numRecvVectorZ.setName("(ZigBee) Received Vector");
//    numLostVectorW.setName("(WiFi) Lost Vector");
//    numLostVectorZ.setName("(ZigBee) Lost Vector");
}
void mConverter::handleMessage(cMessage *msg)
{
    // numSent[0] wifi
    // numSent[1] zigb
    // si la probabilidad es menor a la determinada entonces perdemos un mensaje
    // (1 - fProbability) es para obtener la probabilidad de que no se envíe nada
    if (uniform(0, 1) < (1 - fProbability)) {
        EV << "\"Losing\" message " << msg << endl;
        // mostramos un mensaje bonito en pantalla
        bubble("message lost");
    }
    else {
        // si el mensaje se envió
        // sumamos al contador
        // guardamos en su cVector que generará la gráfica
        numSent[msg->getArrivalGate()->getIndex()]++;
        fnRecordSent(msg->getArrivalGate()->getIndex());

        // muestra en consola quién envía un mensaje
        EV << "Forwarding message " << msg << " on gate[" << msg->getArrivalGate()->getIndex() << "]\n";

        if (msg->getArrivalGate()->getIndex() == 0) // Llega de Wifi
        {
            // dividimos en 18
            // mandamos a ZigBee
            for(int i = 0 ; i < 18; i++)
            {
                char msgname[131];
                sprintf(msgname, "id - %d", i);
                send(new cMessage(msgname), "gate$o", 1);
            }
        }
        else if (msg->getArrivalGate()->getIndex() == 1)// Llega de ZB
        {
            // mandamos a WiFi
            char msgname[2358];
            sprintf(msgname, "To Wifi ");
            send(new cMessage(msgname), "gate$o", 0);
        }
    }
    delete msg;
}
// función para el contador de los mensajes enviados según su cVector
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
//void mConverter::fnRecordRecv(int i)
//{
//    if(i == 0)
//    {
//        numRecvVectorW.record(numReceived[i]);
//    }
//    else
//    {
//        numRecvVectorZ.record(numReceived[i]);
//    }
//}
//void mConverter::fnRecordLost(int i)
//{
//    if(i == 0)
//    {
//        numLostVectorW.record(numLost[i]);
//    }
//    else
//    {
//        numLostVectorZ.record(numLost[i]);
//    }
//}

// función que se hereda de cModule
// se ejecuta cuando se presiona la bandera cuadros
void mConverter::finish()
{
    endTime = simTime() - startTime;
    EV << "Total TIME : " << endTime << endl;
};
// función que se hereda de cModule
// es un relativo a un canvas
// podemos hacer uso de el haciendo un buffer
// el refresh es cada que ocurre un evento dentro del componente
// en este caso cada que entra al handleMessage
void mConverter::refreshDisplay() const
{
    char buf1[100];
    sprintf(
            buf1,
            "WiFi Sent: %ld \n ZigB Sent: %ld ",
            numSent[0], numSent[1]
    );
    getDisplayString().setTagArg("t", 0, buf1);
}

Define_Module(mConverter);
