/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id$
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_MULTI_VAR_GAUSSIAN
#define ROO_MULTI_VAR_GAUSSIAN

#include "RooAbsPdf.h"
#include "RooListProxy.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TVectorD.h"

class RooRealVar;
class RooFitResult ;

class RooMultiVarGaussian : public RooAbsPdf {
public:

  RooMultiVarGaussian() {} ;
  RooMultiVarGaussian(const char *name, const char *title, const RooArgList& xvec, const RooFitResult& fr, Bool_t reduceToConditional=kTRUE) ;
  RooMultiVarGaussian(const char *name, const char *title, const RooArgList& xvec, const RooArgList& mu, const TMatrixDSym& covMatrix) ;
  RooMultiVarGaussian(const char *name, const char *title, const RooArgList& xvec, const TVectorD& mu, const TMatrixDSym& covMatrix) ;
  RooMultiVarGaussian(const char *name, const char *title, const RooArgList& xvec,const TMatrixDSym& covMatrix) ;
  void setAnaIntZ(Double_t z) { _z = z ; }

  RooMultiVarGaussian(const RooMultiVarGaussian& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooMultiVarGaussian(*this,newname); }
  inline virtual ~RooMultiVarGaussian() { }

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ; 
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ; 

  Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const; 
  void initGenerator(Int_t code) ;
  void generateEvent(Int_t code); 

  const TMatrixDSym& covarianceMatrix() const { return _cov ; }
  
  class AnaIntData {
  public:
    TMatrixD    S22bar ;
    Double_t    S22det ;
    vector<int> pmap ;
    Int_t       nint ;
  } ;

  class GenData {
  public:
    TMatrixD    UT ;
    vector<int> omap ;
    vector<int> pmap ;
    TVectorD    mu1 ;
    TVectorD    mu2 ;
    TMatrixD    S12S22I ;
  } ;

  static void blockDecompose(const TMatrixD& input, const vector<int>& map1, const vector<int>& map2, TMatrixDSym& S11, TMatrixD& S12, TMatrixD& S21, TMatrixDSym& S22) ;

protected:

  void decodeCode(Int_t code, vector<int>& map1, vector<int>& map2) const;
  AnaIntData& anaIntData(Int_t code) const ;
  GenData& genData(Int_t code) const ;

  mutable map<int,AnaIntData> _anaIntCache ; //!
  mutable map<int,GenData> _genCache ; //!

  RooListProxy _x ;
  RooListProxy _mu ;
  TMatrixDSym _cov ;
  TMatrixDSym _covI ;
  Double_t    _det ; 
  Double_t    _z ; 

  void syncMuVec() const ;
  mutable TVectorD _muVec ; //! Do not persist

  Double_t evaluate() const ;

private:

  ClassDef(RooMultiVarGaussian,1) // Multivariate Gaussian PDF with correlations
};

#endif
