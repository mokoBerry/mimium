#pragma once

#include <memory>
#include <unordered_map> 
#include <string>

#include "ast.hpp"

using mValue = std::variant<double,std::shared_ptr<Closure>>;

namespace mimium{

class Environment:std::enable_shared_from_this<Environment>{
    std::map<std::string,std::shared_ptr<AST>> variables;
    std::shared_ptr<Environment> parent;
    std::vector<std::shared_ptr<Environment>> children;
    std::string name;
    public:
    Environment():parent(nullptr),name(""){}
    Environment(std::string Name,std::shared_ptr<Environment> Parent):parent(Parent),name(Name){
    }
    AST_Ptr findVariable(std::string key);
    auto& getVariables(){return variables;}
    std::shared_ptr<Environment> createNewChild(std::string newname);
};
struct Closure{
    std::shared_ptr<Environment> env;
    std::shared_ptr<LambdaAST> fun;
    Closure(std::shared_ptr<Environment> Env,std::shared_ptr<LambdaAST> Fun):env(std::move(Env)),fun(std::move(Fun)){};

    std::string to_string();
};



class Interpreter{
    AST_Ptr ast;
    std::shared_ptr<Environment> rootenv;
    std::shared_ptr<Environment> currentenv;
    std::string currentNS;
    bool res;
    public:
    Interpreter():res(false){
        rootenv = std::make_shared<Environment>("root",nullptr);
        currentenv = rootenv; // share
    };
    mValue findVariable(std::string str){ //fortest
        return interpretExpr(currentenv->findVariable(str));
    }
    bool loadAst(AST_Ptr _ast);
    bool interpretTopAst();
    bool interpretAssign(AST_Ptr line);
    bool interpretFdef(AST_Ptr line);

    AST_Ptr interpretVariable(AST_Ptr symbol);

    mValue interpretExpr(AST_Ptr expr);
    mValue interpretBinaryExpr(AST_Ptr expr);

    mValue interpretNumber(AST_Ptr num);
    mValue interpretLambda(AST_Ptr expr);

    mValue interpretFcall(AST_Ptr expr);


    // bool genEventGraph();
};


}

namespace std{
    std::string to_string(std::shared_ptr<mimium::Closure> closure){
        return closure->to_string();
    };
}

