//===-- SPUMCTargetDesc.cpp - Cell SPU Target Descriptions -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Cell SPU specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SPUMCTargetDesc.h"
#include "SPUMCAsmInfo.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Target/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "SPUGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SPUGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SPUGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createSPUMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSPUMCInstrInfo(X);
  return X;
}

extern "C" void LLVMInitializeCellSPUMCInstrInfo() {
  TargetRegistry::RegisterMCInstrInfo(TheCellSPUTarget, createSPUMCInstrInfo);
}

static MCRegisterInfo *createCellSPUMCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitSPUMCRegisterInfo(X, SPU::R0);
  return X;
}

extern "C" void LLVMInitializeCellSPUMCRegisterInfo() {
  TargetRegistry::RegisterMCRegInfo(TheCellSPUTarget,
                                    createCellSPUMCRegisterInfo);
}

static MCSubtargetInfo *createSPUMCSubtargetInfo(StringRef TT, StringRef CPU,
                                                 StringRef FS) {
  MCSubtargetInfo *X = new MCSubtargetInfo();
  InitSPUMCSubtargetInfo(X, TT, CPU, FS);
  return X;
}

extern "C" void LLVMInitializeCellSPUMCSubtargetInfo() {
  TargetRegistry::RegisterMCSubtargetInfo(TheCellSPUTarget,
                                          createSPUMCSubtargetInfo);
}

static MCAsmInfo *createSPUMCAsmInfo(const Target &T, StringRef TT) {
  MCAsmInfo *MAI = new SPULinuxMCAsmInfo(T, TT);

  // Initial state of the frame pointer is R1.
  MachineLocation Dst(MachineLocation::VirtualFP);
  MachineLocation Src(SPU::R1, 0);
  MAI->addInitialFrameState(0, Dst, Src);

  return MAI;
}

extern "C" void LLVMInitializeCellSPUMCAsmInfo() {
  RegisterMCAsmInfoFn X(TheCellSPUTarget, createSPUMCAsmInfo);
}

MCCodeGenInfo *createSPUMCCodeGenInfo(StringRef TT, Reloc::Model RM) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  // For the time being, use static relocations, since there's really no
  // support for PIC yet.
  X->InitMCCodeGenInfo(Reloc::Static);
  return X;
}

extern "C" void LLVMInitializeCellSPUMCCodeGenInfo() {
  TargetRegistry::RegisterMCCodeGenInfo(TheCellSPUTarget,
                                        createSPUMCCodeGenInfo);
}
