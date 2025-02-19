#ifndef LLVM_ICMC_ISEL_LOWERING_H
#define LLVM_ICMC_ISEL_LOWERING_H

#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

namespace ICMCISD {

enum NodeType {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

  RET_FLAG,
  CALL,
  CALLSEQ_END,
  CMP,
  SELECT_CC,
  BRCOND,
  WRAPPER
};

} // end namespace ICMCISD

class ICMCSubtarget;
class ICMCTargetMachine;

class ICMCTargetLowering : public TargetLowering {
private:
  const ICMCSubtarget& Subtarget;

public:
  explicit ICMCTargetLowering(const TargetMachine &TM,
                             const ICMCSubtarget &Subtarget);

  const char *getTargetNodeName(unsigned Opcode) const override;

  template<typename T>
  static void analyzeArguments(const Function *F, const DataLayout *TD,
                               const SmallVectorImpl<T> &Args,
                               SmallVectorImpl<CCValAssign> &ArgLocs,
                               CCState &CCInfo);

  SDValue LowerFormalArguments(
      SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
      const SmallVectorImpl<ISD::InputArg> & Ins, const SDLoc & DL,
      SelectionDAG & DAG, SmallVectorImpl<SDValue> & InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> & Outs,
                      const SmallVectorImpl<SDValue> & OutVals,
                      const SDLoc & DL, SelectionDAG & DAG) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue lowerCallResult(SDValue Chain, SDValue InFlag,
                          CallingConv::ID CallConv, bool IsVarArg,
                          const SmallVectorImpl<ISD::InputArg> &Ins,
                          const SDLoc &DL, SelectionDAG &DAG,
                          SmallVectorImpl<SDValue> &InVals) const;

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  std::pair<unsigned, const TargetRegisterClass *>
  getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                               StringRef Constraint, MVT VT) const override;

  ConstraintType getConstraintType(StringRef Constraint) const override;

  void LowerAsmOperandForConstraint(SDValue Op, std::string &Constraint,
                                    std::vector<SDValue> &Ops,
                                    SelectionDAG &DAG) const override;

  MachineBasicBlock *EmitInstrWithCustomInserter(
      MachineInstr &MI, MachineBasicBlock *MBB) const override;

private:
  SDValue LowerSETCC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;

  SDValue getICMCCmp(SDValue LHS, SDValue RHS, ISD::CondCode CC,
                     SDValue &ICMCcc, SelectionDAG &DAG, SDLoc DL) const;
};

} // end namespace llvm

#endif

