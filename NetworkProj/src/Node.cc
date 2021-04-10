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
#include "MyMessage_m.h"
#include "Node.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
Define_Module(Node);


string Major_string;
int seq_num = -1;
vector <int> hamm_code;
vector <string> window;
vector <string> sendwindow;
vector <string> recwindow;
int counterrec = 0;
int count = 0;
void Hamming_Code(vector <int> in_bits,int m_size);
int ham_calc(int position, int size);
int ack = 0;//this denotes the last frame that was acknowledge by the receiver and I received this ack.
int interval = 0;
//vector <int> majorcounter;
int lengthoftxt = 0;
vector <vector <string>> vecwindow;
vector <int> reversehamm(vector <int> in_bits, int final_size, int& parity_bits);
vector <int> stringtovec(string s,int tete);
void charcount(MyMessage_Base *msg);
void StatCalc();
string vecinttostring( vector <int> ko,int sizem);

int Frame_Gen=0;
int Frame_Drp=0;
int Retrans_Data=0;
int Data_Trans=0;
int ACK_Calc=0;
int NACK_Calc=0;

void Node::initialize()
{
    /*double interval = exponential(1 / par("lambda").doubleValue());
        scheduleAt(simTime() + interval, new MyMessage_Base(""));*/
        InputFile.open("D:\\UNI STUFF\\Third year comp\\first term\\networks\\the_end(proj)\\node" + to_string(getIndex()) + ".txt");///////////////
                 if (!InputFile) {
                     cerr << "Unable to open file datafile.txt";
                     exit(1);   // call system to stop
                     }
                 window.clear();
                 while(!InputFile.eof())
                 {
                     lengthoftxt++;
                     getline(InputFile, Major_string);
                     window.push_back(Major_string);//fill window with all messages
                     sendwindow.push_back("0");
                     recwindow.push_back("0");

                 }
                 vecwindow.push_back(window);
                 InputFile.close();
                 InputFile.open("D:\\UNI STUFF\\Third year comp\\first term\\networks\\the_end(proj)\\node" + to_string(getIndex()) + ".txt");///////////////
                  if (!InputFile) {
                      cerr << "Unable to open file datafile.txt";
                      exit(1);   // call system to stop*/
                      }

}

//TO DO
 /*Create message file and read from it
  * Generate Hub Table
  * Sliding window (selective repeat
  *Channel noise( modification(nice) - LOSS - DuPlIcaTeD - Delays)
  *
  */
//msg,msg,msg,msg,msg,
void Node::handleMessage(cMessage *msg)
{//////////

   // EV<< endl << "the length of my text is " << majorcounter[getIndex()] << endl;
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);

    /////////////////////////////////Sender part/////////////////////////////////////////////////
  //  else
    EV << endl << "seq num of msg is " << mmsg->getSeq_Num() << endl << "last seq is " <<  vecwindow[getIndex()].size() << endl << " mmsg->getM_Type()"<< mmsg->getM_Type()<< endl;
    //{
    if (mmsg->isSelfMessage())
    {
        mmsg->set_reciever(mmsg->get_sender());
        mmsg->set_sender(getIndex());

        if (finished)
        {
            for (int i = selfmsg.size()-1; i >= 0;i--)
            {
                    EV << endl << "deleting " << i <<endl;
                    delete selfmsg[i];
                    selfmsg.pop_back();
            }
            return;
        }
       /* EV << endl <<"Get index is " << getIndex() << endl;
        EV << "The reciever is " << mmsg->get_reciever() << endl << "The sender is " << mmsg->get_sender() << endl;*/
    }
    if (mmsg->get_sender() == getIndex()) { //Host wants to send

        EV<< endl <<"This is the sequence number for finising :"<<mmsg->getSeq_Num()<<"type" <<mmsg->getM_Type() <<endl;
       if (mmsg->getSeq_Num() == vecwindow[getIndex()].size() && mmsg->getM_Type() == 1)//if I got the message and I received an ack for last frame
       {
       EV << endl << "///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////"  << endl;

       /*InputFile.close();

       InputFile.open("F:\\dewloads\\msg.txt");///////////////
       if (!InputFile) {
           cerr << "Unable to open file datafile.txt";
           exit(1);   // call system to stop
           }*/

       seq_num = -1;
       counterrec = 0;
       mmsg->setSeq_Num(seq_num);
       EV << endl << "I am node " + to_string(getIndex()) + " and I have just finished sending" << endl;
       mmsg->setM_Type(4);
       finished = true;
       StatCalc();
       send(mmsg,"outs");
       return;
       }
        if(!InputFile.eof() || mmsg->getM_Type() == 2)//nack or file still open
        {
            if(mmsg->getM_Type() == 1)//ack received, retransmit otherwise
            {
                ack = mmsg->getSeq_Num();
                int rand5;
                rand5 = uniform(0,10);
                EV << "windowsize is " << vecwindow[getIndex()].size() << endl;
                EV << "rand5 is " << rand5 << endl;
                if (rand5 < 5)
                {
                    EV<<"There is going to be a frame drp"<<endl;
                    Frame_Drp++;
                    seq_num++;
                }

                seq_num++;
                if (mmsg->getSeq_Num() > seq_num)
                {
                    seq_num = mmsg->getSeq_Num();
                }

                sendwindow[seq_num] = vecwindow[getIndex()][seq_num];
            }
            else
            {
                Retrans_Data++;
                seq_num = mmsg->getSeq_Num();
                sendwindow[seq_num] = vecwindow[getIndex()][seq_num];
            }


            EV << endl << "t" << endl;
            mmsg->setSeq_Num(seq_num);
                EV<<"String is:"<<sendwindow[seq_num]<<endl;
                EV<<"seqnum "<<mmsg->getSeq_Num()<<endl;
                EV << endl << vecwindow[getIndex()][seq_num] << endl;

            int char_count = vecwindow[getIndex()][seq_num].length()+1;
            EV<<"Character count is "<<char_count<<endl;
            mmsg->setChcount(char_count);

        string finalMess=to_string(char_count)+sendwindow[seq_num];

        EV<<"Final payload is "<<finalMess<<endl;
        mmsg->setM_Payload(finalMess.c_str());
        mmsg->setM_Type(0);//Type 1 means message not ack or nack
        Frame_Gen++;

        vector <int> input_bits;

        input_bits = stringtovec(mmsg->getM_Payload(),strlen(mmsg->getM_Payload()));
        string k = vecinttostring( input_bits,input_bits.size()/8);

        Hamming_Code(input_bits,input_bits.size());
        input_bits = hamm_code;

       int rand1=uniform(0,1)*10;
       int rand2=uniform(0,input_bits.size());

      EV<<"rand is "<<std::to_string(rand1)<<endl;
      EV<<"rand2 is "<<std::to_string(rand2)<<endl;

          if(rand1<6) // prob to delay the message
          {
              EV<<"There is going to be an error in inputs Modification" <<endl ;
              input_bits[rand2] = !input_bits[rand2];
          }
/////////////////////////////////////////////Setting hamming code///////////////////////////////////
        hamm_code=input_bits;
        string s ="";
        for (int i = 0;i < input_bits.size();i++)
            s += (to_string(input_bits[i]));


        mmsg->setM_Payload(s.c_str()); // Message with hamming code and error in it
        getline(InputFile, Major_string);
        //mmsg->setName(to_string(mmsg->get_reciever()).c_str());

        mmsg->setName(to_string(mmsg->get_reciever()).c_str());

        EV<< "Current time is " << simTime() << endl;


        send(mmsg,"outs");
        Data_Trans++;
        MyMessage_Base* amsg = new MyMessage_Base ("1",1);

        amsg->setM_Type(1);
        amsg->set_sender(mmsg->get_reciever());
        amsg->set_reciever(getIndex());

        amsg->setM_Payload("ACK");
        interval++;
        scheduleAt(simTime()+interval, amsg);
        /* double interval = exponential(1 / par("lambda").doubleValue());
               EV << ". Scheduled a new packet after " << simTime() << "s";
              scheduleAt(simTime() + interval, new MyMessage_Base(""));*/

        }
        else
        {
            InputFile.close();
            InputFile.open("D:\\UNI STUFF\\Third year comp\\first term\\networks\\the_end(proj)\\node" + to_string(getIndex()) + ".txt");
            mmsg->setSeq_Num(vecwindow[getIndex()].size());//The file has ended, but I did not receive the ack for the last frame, this means I must have dropped some frames OR some acks were dropped
            //so I must retransmit the frame after the last ack I received
            MyMessage_Base* kmsg = new MyMessage_Base ("1",1);
            kmsg->setM_Type(1);//send to myself a message, that I did not correctly send the frame after the last frame I acknowledged
            kmsg->set_sender(mmsg->get_reciever());
            kmsg->setSeq_Num(vecwindow[getIndex()].size());
            kmsg->set_reciever(getIndex());
            scheduleAt(simTime(),kmsg);
            selfmsg.push_back(kmsg);

        }
     }
    else {
        /////////////////////////////////Reciver part/////////////////////////////////////////////////
        //Check if this is the proper destination

        if (mmsg->get_reciever() == getIndex())
        {
            bubble("Message received");
            vector <int> newmsg;

/////////////////////////////////////////Inserting message as vector int//////////////////////////////////
            for (int i = 0;i < strlen(mmsg->getM_Payload());i++)
                newmsg.push_back((int)(mmsg->getM_Payload()[i]-48));

            vector <int> tite;// reverse the hamcode
            int parity_size;
            tite = reversehamm(newmsg,newmsg.size(),parity_size);
            string s = vecinttostring(tite,tite.size()/8);


            EV<<"Before Corrction"<< endl<< s << endl ;//<< parity_size << endl;

            int l = 0, par = 0;

            bitset <8> k(0);

            while ( (int)pow(2,par) < newmsg.size())
            {
                k[l]=ham_calc(pow(2,par) , newmsg.size());
                l++;
                par++;
            }

            for (int i = l-1;i >= 0;i--)

            {
                EV << k[i];
            }

            EV<< endl;

////////////////////////////////////////// Error Correction ///////////////////////////////////////////
            if(((k.to_ulong())) > 0)
                newmsg[(k.to_ulong())-1] = !newmsg[(k.to_ulong())-1];

            tite = reversehamm(newmsg,newmsg.size(),parity_size);
            s = vecinttostring(tite,tite.size()/8);

            EV<<"After correction"<<endl<< s <<endl;

            hamm_code.clear();
            //seq_num = mmsg->getSeq_Num();
            /*if(((k.to_ulong())) > 0 )
            {
                 mmsg->setM_Payload("NACK");
                 EV<< mmsg->getM_Payload();
                 mmsg->setM_Type(2);

            }
            else
            {
                recwindow[mmsg->getSeq_Num()] = window[mmsg->getSeq_Num()];//[heyoo,    ,
                mmsg->setM_Payload("ACK");                                 //         .
                EV<< mmsg->getM_Payload();
                mmsg->setM_Type(1);
            }//[----,----,----,----,----,----] [*/

            recwindow[mmsg->getSeq_Num()] = vecwindow[mmsg->get_sender()][mmsg->getSeq_Num()];
            EV << endl << "window[mmsg->getSeq_Num() is "<< vecwindow[mmsg->get_sender()][mmsg->getSeq_Num()] << endl << "recwindow[counterrec] is " << recwindow[counterrec] << endl;

            if (recwindow[counterrec] == vecwindow[mmsg->get_sender()][mmsg->getSeq_Num()])
            {
                counterrec++;

                ACK_Calc++;
                mmsg->setM_Payload(("ACK" + to_string(counterrec-1)).c_str());
                /*if (window.size()/7 == counterrec)
                {
                    counterrec = 0;
                }*/
                EV<< mmsg->getM_Payload();
                mmsg->setM_Type(1);
            }
            else
            {
                NACK_Calc++;
                mmsg->setSeq_Num(counterrec);
                mmsg->setM_Type(2);
                mmsg->setM_Payload(("NACK" + to_string(counterrec)).c_str());
                EV<< mmsg->getM_Payload();
                mmsg->setM_Type(2);

            }
            for (int i = vecwindow[mmsg->get_sender()].size();i >= 0;i--)
            {
                if (recwindow[i] == "0")
                {
                    counterrec = i;
                }
            }

            if (counterrec >= vecwindow[mmsg->get_sender()].size())
            {
                for (int i = 0;i < vecwindow[mmsg->get_sender()].size();i++)
                {
                recwindow[i] = "0";
                }
              //  counterrec = vecwindow[mmsg->get_sender()].size()-1;
                EV<<endl<< "Setting the counterrec = vecwindow[mmsg->get_sender()].size()"<<vecwindow[mmsg->get_sender()].size()<<endl;
            }
            mmsg->setSeq_Num(counterrec);
            if (counterrec == vecwindow[mmsg->get_sender()].size())
            {
                counterrec = 0;
                mmsg->setM_Type(1);
            }
            EV << endl << "window.size " << vecwindow[mmsg->get_sender()].size() << " , sendwindow " << sendwindow.size() << " , recwindow " << vecwindow[mmsg->get_sender()].size() << endl;


            mmsg->setName(to_string(mmsg->get_sender()).c_str());
            EV <<"Rec window has " << endl;
                            for(int i = 0;i < vecwindow[mmsg->get_sender()].size();i++)
                                EV << recwindow[i] << endl;
            EV<< endl<< "frame number needed " << mmsg->getSeq_Num()<<endl;
            //randomize delay
            int rand2 = uniform(0,10);
            EV<< endl<<"rand2 > 7 ?" <<rand2 << endl;
            if (rand2 > 7)//ack rand
            {
                EV<<"There is going to be a ack/nack drp";
                send(mmsg,"outs");
            }
        }
            else
            {
                 bubble("Wrong destination");
            }

       // delete msg;
    }
   // }
}

string vecinttostring( vector <int> ko,int sizem)
{

    vector <bitset<8>> l;
    int k = 0;
    bitset <8> b;
            for (int i = 0; i < sizem;i++)
            {

                for (int j = 7; j >= 0;j--)
                {

                 b[j] = ko[k];//bitset has no method called push back
                 k++;
                }
                l.push_back(b);

            }
       /*     for (int i = 0;i < l.size();i++)
        {
                EV << l[i];
        }
            EV << endl;*/
          /*for (int i = 0;i < v.size();i++)
                EV << static_cast<unsigned char>(v[i].to_ulong());*/
          string t;
          for (int i = 0;i < l.size();i++)
                      t += (static_cast<unsigned char>(l[i].to_ulong()));
          return t;
    }

vector <int> stringtovec(string s,int size)
{
    int sizemsg = size;
            vector <bitset<8>> v;//8 bits each char represented by them
            for(int i=0;i<sizemsg;i++)
                v.push_back(s[i]);
            vector <int> input_bits;
            for (int i = 0;i < sizemsg;i++)
            {
                for (int j = 7;j >= 0;j--)
                {
                    input_bits.push_back(v[i][j]);

                }
            }
            return input_bits;
}

vector <int> reversehamm(vector <int> in_bits, int final_size ,int& parity_bits)
{
    //79,72
    int parnum = 0;
    while( (int)pow(2,parnum) < final_size)
    {
        parnum++;
    }
    vector <int> vek;
    int k = 0;
for (int i = 0; i < final_size ; i++) { // filling the final input and parrity bits arrays

            if (i != ((int)pow(2, k) - 1)) {
                vek.push_back(in_bits[i]);
               // hamm_code[i] = in_bits[j];

            }
            else {
                k++;
            }
}
parity_bits = k;
return vek;
}
void charcount(MyMessage_Base *msg)
{
    int char_count = sizeof(msg->getM_Payload())+1;
    msg->setChcount(char_count);
        //3) define the vector of the bitsets
        //4) append the charcount to the vector

}

int ham_calc(int position, int size)
{
    int count = 0, i, j;
    i = position - 1;

    // Traverse to store Hamming Code
    while (i < size) {

        for (j = i; j < i + position; j++) {

            // If current pos is 1
            if (hamm_code[j] == 1)
                count++;// 1
        }

        // Update i
        i = i + 2 * position;
    }

    if (count % 2 == 0)
        return 0;
    else
        return 1;
}

void Hamming_Code(vector <int> in_bits,int m_size)
{

    int r_bits = 0 ,Parity_bits = 0, final_size,j = 0,k = 0 ;

    while(m_size + r_bits + 1 > (int)pow(2,r_bits))
    {
        Parity_bits++;
        r_bits++;
    }

        final_size = Parity_bits + m_size;

    for (int i = 0; i <final_size ; i++) { // filling the final input and parrity bits arrays

            // Update the hamming code
            if (i == ((int)pow(2, k) - 1)) {
            hamm_code.push_back(0);//01101100

                k++;
            }
            // Update the code[r_bits] to the
            // input character at index j
            else {
                hamm_code.push_back(in_bits[j]);

                j++;
            }
        }
        // Traverse and update the
        // hamming code
    for (int i = 0; i < Parity_bits; i++) {

            // Find current position
            int position = (int)pow(2,i);

            // Find value at current position
            int value = ham_calc(position, final_size);

            // Update the code

            hamm_code[position-1] = value;
        }
}

void StatCalc()
{
  EV<<"Total number of generated frames is: "<< Frame_Gen<<endl;
  EV<<"Total number of dropped frames is: "<< Frame_Drp<<endl;
  EV<<"Total number of retransmitted frames is: "<< Retrans_Data<<endl;

  float useful =  Data_Trans;

  float Total_Data = ACK_Calc + NACK_Calc + Retrans_Data + ACK_Calc;

  double percentage = useful/Total_Data*100;

  EV<<"Percentage of useful data transmitted= "<< percentage<<"%"<<endl;
}


