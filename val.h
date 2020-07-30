#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>

using namespace std;


enum ValType { VINT, VSTR, VERR };

class Value {
    ValType	T;
    int		I;
    string	S;

public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}

    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }

    int GetInt() const { if( IsInt() ) return I; throw "control expression not an integer";	}
    string GetStr() const { if( IsStr() ) return S; throw "Value not a string"; }

    Value operator+(const Value& a) const {

        if( IsInt() && a.IsInt() )
            return Value(GetInt() + a.GetInt());

        else if( IsStr() && a.IsStr() )
            return Value(GetStr() + a.GetStr());

        else{throw "Type mismatch for arguments of +";}

    }
    // subtract op from this
    Value operator-(const Value& a) const {


        if( IsInt() && a.IsInt() )
            return Value(GetInt() - a.GetInt());

        else if( a.IsStr() ){
            
           	auto search = S.find(a.GetStr());
        
            string str = S;
            
            if(search != string::npos)
                str.erase(search, a.GetStr().length());

            return str;
            
        }

        else{throw "Type mismatch on operands of -";}
                     

    }


    // multiply this by op
    Value operator*(const Value& a) const {
 
        if( IsInt() && a.IsInt() )
            return Value(GetInt() * a.GetInt());
        
        else if(IsStr() || a.IsStr()){
            
            if(IsStr() && a.IsStr())
                throw "Type error on multiplication";
        	
            else if(IsStr()){
                
                string mult;
                int times = a.GetInt();

                if(times < 0)
                    throw "Repetition count less than 0";

                while(times > 0){	
                    mult += GetStr();
                    times--;
                }
                
                return Value(mult);
            }
            
            else if(a.IsStr()){
                           	
                string mult;
                int times = GetInt();
                
                if(times < 0)
                    throw "Repitition count less than 0";
                
                while(times > 0){
                	mult += a.GetStr();
                    times --;
                }
                
                return Value(mult);
   
            }
            
        }
        
        throw "Multiplication type error";


    }

    // divide this by op
    Value operator/(const Value& a) const {

        if(a.GetInt() == 0){
            throw "Divide by zero error";
        }

        else if( IsInt() && a.IsInt() ){
            
            if(GetInt() < a.GetInt())
                return Value(0);
        
            return Value(GetInt() / a.GetInt());}


        throw "Type error";

        
    }

    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR" << endl;
        return out;
    }
};

#endif