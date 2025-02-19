#ifndef LLVM_ICMC_INSTR_PRINTER_H
#define LLVM_ICMC_INSTR_PRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class ICMCInstPrinter : public MCInstPrinter {
public:
  ICMCInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                 const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;
  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                         const MCSubtargetInfo &STI, raw_ostream &OS) override;

  static const char *getPrettyRegisterName(unsigned RegNo,
                                           MCRegisterInfo const &MRI);
private:
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &O);

  static const char *getRegisterName(unsigned RegNo);
};

} // end namespace llvm

#endif
