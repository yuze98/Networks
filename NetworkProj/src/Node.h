#ifndef __MESH_NODE_H_
#define __MESH_NODE_H_

#include <omnetpp.h>
#include <bitset>
#include <vector>
#include <fstream>
#include <string.h>


using namespace omnetpp;
using namespace std;


/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:

    ifstream InputFile;
    string str;
    bool finished = false;
    vector <MyMessage_Base*> selfmsg;
};

#endif
