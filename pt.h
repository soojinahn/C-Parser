
#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

#include "parse.h"
#include "lex.h"
#include "val.h"

extern map<string, Value> symbols;

class Pt {
protected:
    int	linenum;
    Pt	*left;
    Pt	*right;

public:
    Pt(int linenum, Pt *l = 0, Pt *r = 0)
            : linenum(linenum), left(l), right(r) {}

    virtual ~Pt() {
        delete left;
        delete right;
    }
    
    virtual Value Eval(map <string, Value>& sym) const = 0;

    int GetLineNumber() const { return linenum; }

    int PlusCount() const {
        int count = 0;
        if(left) count += left->PlusCount();
        if(right) count += right->PlusCount();
        if(isPlus())
            count++;
        return count;
    }
    int EqCount() const {
        int count = 0;
        if(left) count += left->EqCount();
        if(right) count += right->EqCount();
        if(isEq())
            count ++;
        return count;
    }
    int MaxDepth() const {
        int l = 0;
        int r = 0;
        if(left) l+= left->MaxDepth();
        if(right) r+= right->MaxDepth();

        return (l > r ? l:r) + 1;
    }

    virtual bool isPlus() const { return false; }
    virtual bool isEq() const { return false; }
    virtual bool isIdent() const { return false; }
    virtual bool isVar() const { return false; }
    virtual string getId() const { return ""; }
    
    Pt* GetLeft() const { return left; }
    Pt* GetRight() const { return right; }

};

class StmtList : public Pt {
public:
    StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    
    Value Eval(map<string,Value>& symbols) const {
        
        GetLeft()->Eval(symbols);
        if( right )
            GetRight()->Eval(symbols);
        return Value();
    }
};

class Print : public Pt {
public:
    Print(int line, Pt *e) : Pt(line, e) {}
    
   	Value Eval(map<string,Value>& symbols) const {
        
        Value val = GetLeft()->Eval(symbols);
		cout << val;
        return Value(val);
    }
    
};

class Println : public Pt {
public:
    Println(int line, Pt *e) : Pt(line, e) {}
    
    Value Eval(map<string,Value>& symbols) const {
        
        Value val = GetLeft()->Eval(symbols);
    	cout << val << endl;
        return Value(val);
    }
};

class Repeat : public Pt {
public:
    Repeat(int line, Pt *l, Pt *r) : Pt(line, l, r){}
    Value Eval(map<string,Value>& symbols) const {
    	
        auto loop = GetLeft()->Eval(symbols);
        if(loop.IsInt()){
        	
            for(int i = loop.GetInt(); i>0 ; i--)
                symbols[GetLeft()->getId()] = GetRight()->Eval(symbols);
            
            return Value(symbols[GetLeft()->getId()]);
        }
        
        throw "at " + to_string(GetLineNumber() +1) + ": control expression not an integer";
        
       }
               
};

class Assign : public Pt {
	string id;
public:
    Assign(int line, Pt *l, Pt *r) : Pt(line, l, r){ id = getId();}
    bool isEq() const{return true;}
    
    
    Value Eval(map<string,Value>& symbols) const {
        if(GetLeft()->isIdent()){
        	Value right = GetRight()->Eval(symbols);
            string left = GetLeft()->getId();
            return Value(symbols[left] = right);
        }
        
        return Value();
        
    }
    
    
};

class PlusExpr : public Pt {
public:
    PlusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    bool isPlus() const{return true;}
    
    Value Eval(map<string,Value>& symbols) const {
    	
        Value leftNode = GetLeft()->Eval(symbols);
        Value rightNode = GetRight()->Eval(symbols);
        
        return leftNode + rightNode;
    }
    
};

class MinusExpr : public Pt {
public:
    MinusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    
   	Value Eval(map<string,Value>& symbols) const {
    	
        Value leftNode = GetLeft()->Eval(symbols);
        Value rightNode = GetRight()->Eval(symbols);
        
        return leftNode - rightNode;
    }

};

class TimesExpr : public Pt {
public:
    TimesExpr(int line, Pt *l, Pt *r) : Pt(line, l,r) {}
    
    Value Eval(map<string,Value>& symbols) const {
    
        Value leftNode = GetLeft()->Eval(symbols);
        Value rightNode = GetRight()->Eval(symbols);
        
        if(leftNode.IsInt() || rightNode.IsInt())
            return leftNode * rightNode;
        else
            throw "at " + to_string(GetLineNumber()+1) + ": Type error on multiplication";
    }    

};

class DivideExpr : public Pt {
public:
    DivideExpr(int line, Pt *l, Pt *r) : Pt(line,l,r) {}
    
    Value Eval(map<string,Value>& symbols) const {
    	
        
        Value leftNode = GetLeft()->Eval(symbols);
        Value rightNode = GetRight()->Eval(symbols);
        
        return leftNode / rightNode;
    }

};

class IConst : public Pt {
    int val;

public:
    IConst(Tok& t) : Pt(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }
    
    Value Eval(map<string,Value>& symbols) const {
    	return Value(val);
    }
};

class SConst : public Pt {
    string val;

public:
    SConst(Tok& t) : Pt(t.GetLinenum()) {
        val = t.GetLexeme();
    }
    string getId() const {return val;}
    
    Value Eval(map<string,Value>& symbols) const {
    	return Value(val);
    }
};

class Ident : public Pt {
    string val;

public:
    Ident(Tok& t) : Pt(t.GetLinenum()){
        val = t.GetLexeme();
    }
    string getId() const {return val;}
    
    bool isIdent() const {return true;}
    
    Value Eval(map<string,Value>& symbols) const {
    	
        if(symbols.find(val) != symbols.end())
        	return symbols[val];
        
        
        else{
        	throw "at " + to_string(GetLineNumber() + 1) + ": Variable used but not assigned";
        }

    }

};

#endif /* PT_H_ */
