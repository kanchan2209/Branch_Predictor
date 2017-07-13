#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
using namespace std;

int m;
int SCArray[1048576];

int main(int argc, char* argv[])
{
  ifstream confm;              //Retrieving value of m from config file
  confm.open(argv[1]);
  while(!confm.eof())
  {
    confm>>m;                     
    //cout<<m<<endl; //debug
    for(int i=0; i<1048576;i++)
    {
      SCArray[i]=3;           //initializing all the 2 bit counters to be of value taken. Here we are considering the states 00,01,10,11 to be 0,1,2,3.
    }
  }

  ifstream traces;
  ofstream tracesout;
  string outname;
  outname = string(argv[2]) + ".out"; 
  traces.open(argv[2]);
  tracesout.open(outname.c_str());
  string line;
  string baddr;
  string action;
  unsigned int addr;
  bitset<32> bitaddr;
  int index_int;
  float tot =0;
  float miss=0;
  float mspred;
  int pred = 1;
  int action_int;
  if (traces.is_open()&&tracesout.is_open())
  {    
        while (getline (traces,line)) // read trace file
        {   
            tot++;
            istringstream iss(line); 
            if (!(iss >> baddr >> action)) {break;}
            stringstream saddr(baddr);
            saddr >> std::hex >> addr;
            //cout<<addr<<endl; //debug
            bitaddr = bitset<32> (addr);
            //cout<<bitaddr<<endl; //debug
            //cout<<bitaddr.to_string()<<endl; //cout in front is for debug
            string index = bitaddr.to_string().substr(31-m+1,32);                   //retrieving the m LSB bits from the branch address as the index to the counter
            //cout<<index<<endl; //debug 
            bitset<32> index_bitset(index);
            //cout<<index_bitset<<endl;//debug
            index_int = (int)(index_bitset.to_ulong());
            //cout<<index_int<<endl; //debug
            bitset<32> action_bitset(action);
            action_int = (int)(action_bitset.to_ulong()); 
            //cout<<"Action in int "<<action_int<<endl; //debug
            //cout<<"Prediction "<<pred<<endl; //debug
            //cout<<SCArray[index_int]<<endl; //debug
            if(SCArray[index_int]==3|SCArray[index_int]==2)  //Obtaining the prediction value on the basis of the counter states
               pred=0;
             else
               pred=1;
            if(action_int==1)                               // If action is taken, ie the branch is taken, following is the case with the four states in the counter
            {
             //cout<<"Reached IF ACTION 1"<<endl;//debug
             int a = SCArray[index_int];
             //cout<<"Current value in this pos "<<a<<endl;//debug
              
             switch(a)                                      //the four counter states are given as switch cases
             {
              case 0: miss++;                                //miss prediction here at 00(which is prediction not taken from counter)  
                      SCArray[index_int]=1; 
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl; //debug
                      break;
              case 1: miss++;                                //miss prediction here at 01(which is prediction not taken from counter) 
                      SCArray[index_int]=3; 
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl;//debug
                      break;
              case 2: SCArray[index_int]=3;                  //no chance of miss prediction with state as 10 with branch action taken
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl;//debug
                      break;
              case 3: SCArray[index_int]=3; 
                      //cout<<SCArray[index_int]<<endl;//debug      //no chance of miss prediction with state as 11 with branch action taken 
                      //cout<<pred<<endl;//debug
                      break;
              default: break;        
             }           
            } 
            if(action_int==0)              //If action is not taken, ie the branch is not taken, following is the case with the four states in the counter
            {
             //cout<<"Reached ELSE IF ACTION 0"<<endl;//debug
             int b = SCArray[index_int];
             //cout<<"Current value in this pos "<<b<<endl;//debug
             switch(b)                    //the four counter states are given as switch cases
             {
              case 0: SCArray[index_int]=0;    //no chance of miss prediction with state as 00 with branch not taken
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl;//debug
                      break;
              case 1: SCArray[index_int]=0;    //no chance of miss prediction with state as 01 with branch not taken
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl;//debug
                      break;
              case 2: miss++;                  //miss prediction here at 10(which is prediction taken from counter)
                      SCArray[index_int]=0; 
                      //cout<<SCArray[index_int]<<endl;//debug
                      //cout<<pred<<endl; //debug
                      break;
              case 3: miss++;                 //miss prediction here at 11(which is prediction taken from counter)
                      SCArray[index_int]=2; 
                      //cout<<SCArray[index_int]<<endl;//debug                  
                      //cout<<pred<<endl;//debug
                      break; 
              default: break;        
             }             
            }               
            
            tracesout<<pred<<endl; //Outputing the prediction values in the traceout file
        }
          
       traces.close();      //close respective files
       tracesout.close();
       //cout<<"Total Predictions "<<tot<<endl;     //Debug. Calculating the total predictions and miss predictions to obtain the miss prediction rate. 
       //cout<<"Miss "<<miss<<endl; 
       mspred=(miss/tot)*100;
       //cout<<"Mis %"<<mspred;  
  }
  else
     cout<<"Unable to Open Trace or Traceout File"<<endl;
  return 0;
}




