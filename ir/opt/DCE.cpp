#include "../../include/ir/opt/DCE.hpp"

bool DCE::Run() {
  /*
  要求：
  1. 顺序遍历function中的每个基本块,并顺序遍历基本块中的每个指令
  2. 如果一个指令没有副作用, 并且是一个"死"指令，那么就删除这个指令
  */
  bool flag = false;
  for(auto bb : (*func)) {
    for(auto inst : (*bb)) {
      if(!HasSideEffect(inst) && inst->GetUserListSize() == 0) {
        RemoveInst(inst);
        flag = true;
      }
    }
  }
  return flag;
}

bool DCE::HasSideEffect(User *inst) {
  /*
  要求：
  1. 判断一个指令是否有副作用
  2. 如果有副作用, 返回true, 否则返回false
  */
  //如果是终结指令
  if(inst->IsTerminateInst()||inst->IsCondInst()||inst->IsUncondInst())
    return true;
  //如果是函数调用
  else if(inst->id==User::OpID::Call)
    return true;
  //如果是内存操作指令
  else if(inst->id>=User::OpID::Alloca&&inst->id<=User::OpID::Memcpy)
    return true;
  else
    return false;
  /*
  思考：
  1. 什么情况下一条指令会有副作用？
  2. 如何判断一条指令是否有副作用？
  */
}

void DCE::RemoveInst(User *inst) {
  /*
  要求：
  正确删除一条指令, 并使用User类中正确的函数维护关系。
  */

  inst->ClearRelation();         
  inst->EraseFromParent(); 
  
  /*
  思考：
  1. 如何删除一条指令？
  2. 如何维护一条指令的Use\User\Usee关系?
  */
}