#include <iostream>
#include <fstream>
#include "parse.h"
#include "lex.h"


using namespace std;


int main (int argc, char* argv[]){

    ifstream file;
    istream *in = &cin;

    if(argc > 2){
        cout << "ONLY ONE FILE NAME ALLOWED" << endl;
        return 0;
    }

    else if(argc == 2){

        file.open(argv[1]);
        if(!file.is_open()){
            cout << "CANNOT OPEN " << argv[1] << endl;
            return 0;
        }

        in = &file;


    }
    
    map<string,Value> symbols;
    //map<string, bool> var;
   

    int lineNum = 0;

    Pt *prog = Prog(*in, lineNum);
    
    if(prog == 0) 
        return 0;
    
    try{        
        prog->Eval(symbols);  
        //prog->CheckingAssign(var);

    }
    
    catch(string& error){
    	cout << "RUNTIME ERROR " << error << endl;
    }

    return 0;

    

}
