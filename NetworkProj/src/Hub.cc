//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
#include <string>
#include "Hub.h"
#include "MyMessage_m.h"
#include <iostream>
using namespace std;

Define_Module(Hub);

vector <vector<int>> vek;
int rando = 1;

void Hub::initialize()
{
    MyMessage_Base *mmsg = new MyMessage_Base ("Yalla");

    int table_size = 10 ; //node numbers

    for(int i=0;i<table_size;i++)
    {
        vector <int> vek_int;
        int rand_pair1=uniform(0,7);
        int rand_pair2=uniform(0,7);

        while(rand_pair1 == rand_pair2)
        {
             rand_pair1=uniform(0,7);
        }
            vek_int.push_back(rand_pair1);
            vek_int.push_back(rand_pair2);

            vek.push_back(vek_int);

    }

    for(int i=0;i<vek.size();i++){
        for(int j =0;j<2;j++)
            EV << vek[i][j];

        EV << endl;
    }

    mmsg->set_sender(vek[0][0]);
    mmsg->set_reciever(vek[0][1]);

    mmsg->setM_Type(1);
    send(mmsg, "outs", mmsg->get_sender());
}
void Hub::handleMessage(cMessage *msg)
{
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);

    if(mmsg->getM_Type() == 4)//end of fileeeeeeeeee
    {
        EV<<endl<<"yeeeeeeeeeeeeeeeeeeeeeeyeeeeeeeeeeeeeee"<<endl;

        mmsg->set_sender(vek[rando][0]);
        mmsg->set_reciever(vek[rando][1]);
        mmsg->setM_Type(1);
        rando++;
        send(mmsg, "outs", mmsg->get_sender());
    }
    else
    {
        EV << mmsg->get_sender();

    //set name of receiver
        int dest;

        dest = atoi(mmsg->getName());

        send(mmsg, "outs", dest);
    }
}



