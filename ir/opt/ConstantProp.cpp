#include "../../include/ir/opt/ConstantProp.hpp"
#include <cmath>
#include <set>

bool ConstantProp::Run() {
  /*
  要求：
  1. 顺序遍历function中的每个基本块,并顺序遍历基本块中的每个指令
  2. 如果一个指令是常量传播的结果，那么就将这个指令替换为常量，并删除这个指令
  */
 for(auto bb:_func->GetBasicBlock()) 
 {
    for(auto inst:*bb)
    {
        if(Value* tem=ConstFoldInst(inst))
        {
            inst->RAUW(tem);
            inst->ClearRelation();
            inst->EraseFromParent();
        }
    }
 }
  return true;
}

Value *ConstantProp::ConstFoldInst(User *inst) {
  /*
  要求：
  1. 考虑一条指令是否能进行常量折叠
  2. 如果能够进行常量折叠，那么返回折叠后的常量
  3. 正确判断指令的类型，对于不同的类型需要有不同的处理方法
  这个过程可以自行在 ConstantProp类中增加函数来实现功能
  */
  switch (inst->GetInstId()) 
  {
    case User::OpID::Add:
    {
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRInt*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() + op2->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Sub:
    {
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRInt*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() - op2->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Mul:
    {
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRInt*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() * op2->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Div:
    {
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRInt*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() / op2->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Mod:
    {
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRInt*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() % op2->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::And:
    {
        auto op1 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() && op2->GetVal();
            return ConstIRBoolean::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Or:
    {
        auto op1 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() || op2->GetVal();
            return ConstIRBoolean::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Xor:
    {
        auto op1 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(0));
        auto op2 = dynamic_cast<ConstIRBoolean*>(inst->GetOperand(1));
        if(op1 && op2)
        {
            auto res = op1->GetVal() ^ op2->GetVal();
            return ConstIRBoolean::GetNewConstant(res);
        }
        break;
    }
    case User::OpID::Ret:
    {
        if(inst->GetUserListSize() == 0)
            return nullptr;
        auto op1 = dynamic_cast<ConstIRInt*>(inst->GetOperand(0));
        if(op1)
        {
            auto res = op1->GetVal();
            return ConstIRInt::GetNewConstant(res);
        }
        break;
    }
    //other instructions
    
  }
  return nullptr;
}
