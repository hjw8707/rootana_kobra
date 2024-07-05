// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME KoBRA_DictOnly
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "include/TPPACData.hxx"
#include "include/TLPPACData.hxx"
#include "include/TPlasticData.hxx"
#include "include/TSSDData.hxx"
#include "macros/kobra.hxx"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_TPPACData(void *p = 0);
   static void *newArray_TPPACData(Long_t size, void *p);
   static void delete_TPPACData(void *p);
   static void deleteArray_TPPACData(void *p);
   static void destruct_TPPACData(void *p);
   static void streamer_TPPACData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TPPACData*)
   {
      ::TPPACData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TPPACData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TPPACData", ::TPPACData::Class_Version(), "include/TPPACData.hxx", 6,
                  typeid(::TPPACData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TPPACData::Dictionary, isa_proxy, 16,
                  sizeof(::TPPACData) );
      instance.SetNew(&new_TPPACData);
      instance.SetNewArray(&newArray_TPPACData);
      instance.SetDelete(&delete_TPPACData);
      instance.SetDeleteArray(&deleteArray_TPPACData);
      instance.SetDestructor(&destruct_TPPACData);
      instance.SetStreamerFunc(&streamer_TPPACData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TPPACData*)
   {
      return GenerateInitInstanceLocal((::TPPACData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TPPACData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TLPPACData(void *p = 0);
   static void *newArray_TLPPACData(Long_t size, void *p);
   static void delete_TLPPACData(void *p);
   static void deleteArray_TLPPACData(void *p);
   static void destruct_TLPPACData(void *p);
   static void streamer_TLPPACData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TLPPACData*)
   {
      ::TLPPACData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TLPPACData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TLPPACData", ::TLPPACData::Class_Version(), "include/TLPPACData.hxx", 6,
                  typeid(::TLPPACData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TLPPACData::Dictionary, isa_proxy, 16,
                  sizeof(::TLPPACData) );
      instance.SetNew(&new_TLPPACData);
      instance.SetNewArray(&newArray_TLPPACData);
      instance.SetDelete(&delete_TLPPACData);
      instance.SetDeleteArray(&deleteArray_TLPPACData);
      instance.SetDestructor(&destruct_TLPPACData);
      instance.SetStreamerFunc(&streamer_TLPPACData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TLPPACData*)
   {
      return GenerateInitInstanceLocal((::TLPPACData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TLPPACData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TPlasticData(void *p = 0);
   static void *newArray_TPlasticData(Long_t size, void *p);
   static void delete_TPlasticData(void *p);
   static void deleteArray_TPlasticData(void *p);
   static void destruct_TPlasticData(void *p);
   static void streamer_TPlasticData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TPlasticData*)
   {
      ::TPlasticData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TPlasticData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TPlasticData", ::TPlasticData::Class_Version(), "include/TPlasticData.hxx", 6,
                  typeid(::TPlasticData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TPlasticData::Dictionary, isa_proxy, 16,
                  sizeof(::TPlasticData) );
      instance.SetNew(&new_TPlasticData);
      instance.SetNewArray(&newArray_TPlasticData);
      instance.SetDelete(&delete_TPlasticData);
      instance.SetDeleteArray(&deleteArray_TPlasticData);
      instance.SetDestructor(&destruct_TPlasticData);
      instance.SetStreamerFunc(&streamer_TPlasticData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TPlasticData*)
   {
      return GenerateInitInstanceLocal((::TPlasticData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TPlasticData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TSSDData(void *p = 0);
   static void *newArray_TSSDData(Long_t size, void *p);
   static void delete_TSSDData(void *p);
   static void deleteArray_TSSDData(void *p);
   static void destruct_TSSDData(void *p);
   static void streamer_TSSDData(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TSSDData*)
   {
      ::TSSDData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TSSDData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TSSDData", ::TSSDData::Class_Version(), "include/TSSDData.hxx", 6,
                  typeid(::TSSDData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TSSDData::Dictionary, isa_proxy, 16,
                  sizeof(::TSSDData) );
      instance.SetNew(&new_TSSDData);
      instance.SetNewArray(&newArray_TSSDData);
      instance.SetDelete(&delete_TSSDData);
      instance.SetDeleteArray(&deleteArray_TSSDData);
      instance.SetDestructor(&destruct_TSSDData);
      instance.SetStreamerFunc(&streamer_TSSDData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TSSDData*)
   {
      return GenerateInitInstanceLocal((::TSSDData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TSSDData*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_KOBRA(void *p = 0);
   static void *newArray_KOBRA(Long_t size, void *p);
   static void delete_KOBRA(void *p);
   static void deleteArray_KOBRA(void *p);
   static void destruct_KOBRA(void *p);
   static void streamer_KOBRA(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KOBRA*)
   {
      ::KOBRA *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::KOBRA >(0);
      static ::ROOT::TGenericClassInfo 
         instance("KOBRA", ::KOBRA::Class_Version(), "macros/kobra.hxx", 10,
                  typeid(::KOBRA), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::KOBRA::Dictionary, isa_proxy, 16,
                  sizeof(::KOBRA) );
      instance.SetNew(&new_KOBRA);
      instance.SetNewArray(&newArray_KOBRA);
      instance.SetDelete(&delete_KOBRA);
      instance.SetDeleteArray(&deleteArray_KOBRA);
      instance.SetDestructor(&destruct_KOBRA);
      instance.SetStreamerFunc(&streamer_KOBRA);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KOBRA*)
   {
      return GenerateInitInstanceLocal((::KOBRA*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::KOBRA*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TPPACData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TPPACData::Class_Name()
{
   return "TPPACData";
}

//______________________________________________________________________________
const char *TPPACData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPPACData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TPPACData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPPACData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TPPACData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPPACData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TPPACData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPPACData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TLPPACData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TLPPACData::Class_Name()
{
   return "TLPPACData";
}

//______________________________________________________________________________
const char *TLPPACData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLPPACData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TLPPACData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TLPPACData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TLPPACData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLPPACData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TLPPACData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TLPPACData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TPlasticData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TPlasticData::Class_Name()
{
   return "TPlasticData";
}

//______________________________________________________________________________
const char *TPlasticData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPlasticData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TPlasticData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TPlasticData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TPlasticData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPlasticData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TPlasticData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TPlasticData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TSSDData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TSSDData::Class_Name()
{
   return "TSSDData";
}

//______________________________________________________________________________
const char *TSSDData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSSDData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TSSDData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TSSDData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TSSDData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSSDData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TSSDData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TSSDData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr KOBRA::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *KOBRA::Class_Name()
{
   return "KOBRA";
}

//______________________________________________________________________________
const char *KOBRA::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::KOBRA*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int KOBRA::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::KOBRA*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KOBRA::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::KOBRA*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KOBRA::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::KOBRA*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TPPACData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TPPACData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> tx1;
      R__b >> tx2;
      R__b >> ty1;
      R__b >> ty2;
      R__b >> ta;
      R__b >> x;
      R__b >> y;
      R__b.CheckByteCount(R__s, R__c, TPPACData::IsA());
   } else {
      R__c = R__b.WriteVersion(TPPACData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << tx1;
      R__b << tx2;
      R__b << ty1;
      R__b << ty2;
      R__b << ta;
      R__b << x;
      R__b << y;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TPPACData(void *p) {
      return  p ? new(p) ::TPPACData : new ::TPPACData;
   }
   static void *newArray_TPPACData(Long_t nElements, void *p) {
      return p ? new(p) ::TPPACData[nElements] : new ::TPPACData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TPPACData(void *p) {
      delete ((::TPPACData*)p);
   }
   static void deleteArray_TPPACData(void *p) {
      delete [] ((::TPPACData*)p);
   }
   static void destruct_TPPACData(void *p) {
      typedef ::TPPACData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TPPACData(TBuffer &buf, void *obj) {
      ((::TPPACData*)obj)->::TPPACData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TPPACData

//______________________________________________________________________________
void TLPPACData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TLPPACData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> txl1;
      R__b >> txl2;
      R__b >> txc1;
      R__b >> txc2;
      R__b >> txr1;
      R__b >> txr2;
      R__b >> ty1;
      R__b >> ty2;
      R__b >> ta;
      R__b >> x;
      R__b >> y;
      R__b.CheckByteCount(R__s, R__c, TLPPACData::IsA());
   } else {
      R__c = R__b.WriteVersion(TLPPACData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << txl1;
      R__b << txl2;
      R__b << txc1;
      R__b << txc2;
      R__b << txr1;
      R__b << txr2;
      R__b << ty1;
      R__b << ty2;
      R__b << ta;
      R__b << x;
      R__b << y;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TLPPACData(void *p) {
      return  p ? new(p) ::TLPPACData : new ::TLPPACData;
   }
   static void *newArray_TLPPACData(Long_t nElements, void *p) {
      return p ? new(p) ::TLPPACData[nElements] : new ::TLPPACData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TLPPACData(void *p) {
      delete ((::TLPPACData*)p);
   }
   static void deleteArray_TLPPACData(void *p) {
      delete [] ((::TLPPACData*)p);
   }
   static void destruct_TLPPACData(void *p) {
      typedef ::TLPPACData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TLPPACData(TBuffer &buf, void *obj) {
      ((::TLPPACData*)obj)->::TLPPACData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TLPPACData

//______________________________________________________________________________
void TPlasticData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TPlasticData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> tl;
      R__b >> tr;
      R__b >> al;
      R__b >> ar;
      R__b >> tsum;
      R__b >> tdiff;
      R__b.CheckByteCount(R__s, R__c, TPlasticData::IsA());
   } else {
      R__c = R__b.WriteVersion(TPlasticData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << tl;
      R__b << tr;
      R__b << al;
      R__b << ar;
      R__b << tsum;
      R__b << tdiff;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TPlasticData(void *p) {
      return  p ? new(p) ::TPlasticData : new ::TPlasticData;
   }
   static void *newArray_TPlasticData(Long_t nElements, void *p) {
      return p ? new(p) ::TPlasticData[nElements] : new ::TPlasticData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TPlasticData(void *p) {
      delete ((::TPlasticData*)p);
   }
   static void deleteArray_TPlasticData(void *p) {
      delete [] ((::TPlasticData*)p);
   }
   static void destruct_TPlasticData(void *p) {
      typedef ::TPlasticData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TPlasticData(TBuffer &buf, void *obj) {
      ((::TPlasticData*)obj)->::TPlasticData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TPlasticData

//______________________________________________________________________________
void TSSDData::Streamer(TBuffer &R__b)
{
   // Stream an object of class TSSDData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> ch;
      R__b >> adc;
      R__b >> acal;
      R__b.CheckByteCount(R__s, R__c, TSSDData::IsA());
   } else {
      R__c = R__b.WriteVersion(TSSDData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << ch;
      R__b << adc;
      R__b << acal;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TSSDData(void *p) {
      return  p ? new(p) ::TSSDData : new ::TSSDData;
   }
   static void *newArray_TSSDData(Long_t nElements, void *p) {
      return p ? new(p) ::TSSDData[nElements] : new ::TSSDData[nElements];
   }
   // Wrapper around operator delete
   static void delete_TSSDData(void *p) {
      delete ((::TSSDData*)p);
   }
   static void deleteArray_TSSDData(void *p) {
      delete [] ((::TSSDData*)p);
   }
   static void destruct_TSSDData(void *p) {
      typedef ::TSSDData current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TSSDData(TBuffer &buf, void *obj) {
      ((::TSSDData*)obj)->::TSSDData::Streamer(buf);
   }
} // end of namespace ROOT for class ::TSSDData

//______________________________________________________________________________
void KOBRA::Streamer(TBuffer &R__b)
{
   // Stream an object of class KOBRA.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> tree;
      {
         map<Int_t,Double_t> &R__stl =  brhoMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef int Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      {
         map<Int_t,Double_t> &R__stl =  f1slitMap;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            double R__t2;
            R__b >> R__t2;
            typedef int Value_t;
            std::pair<Value_t const, double > R__t3(R__t,R__t2);
            R__stl.insert(R__t3);
         }
      }
      R__b >> runN;
      R__b >> centBrho;
      R__b >> tofOff;
      R__b >> useF1;
      R__b >> useF2orF3;
      { TString R__str; R__str.Streamer(R__b); gcut = R__str.Data(); }
      R__b.CheckByteCount(R__s, R__c, KOBRA::IsA());
   } else {
      R__c = R__b.WriteVersion(KOBRA::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << tree;
      {
         map<Int_t,Double_t> &R__stl =  brhoMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Int_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      {
         map<Int_t,Double_t> &R__stl =  f1slitMap;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            map<Int_t,Double_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << ((*R__k).first );
            R__b << ((*R__k).second);
            }
         }
      }
      R__b << runN;
      R__b << centBrho;
      R__b << tofOff;
      R__b << useF1;
      R__b << useF2orF3;
      { TString R__str = gcut.c_str(); R__str.Streamer(R__b);}
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_KOBRA(void *p) {
      return  p ? new(p) ::KOBRA : new ::KOBRA;
   }
   static void *newArray_KOBRA(Long_t nElements, void *p) {
      return p ? new(p) ::KOBRA[nElements] : new ::KOBRA[nElements];
   }
   // Wrapper around operator delete
   static void delete_KOBRA(void *p) {
      delete ((::KOBRA*)p);
   }
   static void deleteArray_KOBRA(void *p) {
      delete [] ((::KOBRA*)p);
   }
   static void destruct_KOBRA(void *p) {
      typedef ::KOBRA current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_KOBRA(TBuffer &buf, void *obj) {
      ((::KOBRA*)obj)->::KOBRA::Streamer(buf);
   }
} // end of namespace ROOT for class ::KOBRA

namespace ROOT {
   static TClass *maplEintcOdoublegR_Dictionary();
   static void maplEintcOdoublegR_TClassManip(TClass*);
   static void *new_maplEintcOdoublegR(void *p = 0);
   static void *newArray_maplEintcOdoublegR(Long_t size, void *p);
   static void delete_maplEintcOdoublegR(void *p);
   static void deleteArray_maplEintcOdoublegR(void *p);
   static void destruct_maplEintcOdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,double>*)
   {
      map<int,double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,double>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,double>", -2, "map", 100,
                  typeid(map<int,double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,double>) );
      instance.SetNew(&new_maplEintcOdoublegR);
      instance.SetNewArray(&newArray_maplEintcOdoublegR);
      instance.SetDelete(&delete_maplEintcOdoublegR);
      instance.SetDeleteArray(&deleteArray_maplEintcOdoublegR);
      instance.SetDestructor(&destruct_maplEintcOdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,double> >()));

      ::ROOT::AddClassAlternate("map<int,double>","std::map<int, double, std::less<int>, std::allocator<std::pair<int const, double> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,double>*)0x0)->GetClass();
      maplEintcOdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double> : new map<int,double>;
   }
   static void *newArray_maplEintcOdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,double>[nElements] : new map<int,double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOdoublegR(void *p) {
      delete ((map<int,double>*)p);
   }
   static void deleteArray_maplEintcOdoublegR(void *p) {
      delete [] ((map<int,double>*)p);
   }
   static void destruct_maplEintcOdoublegR(void *p) {
      typedef map<int,double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,double>

namespace {
  void TriggerDictionaryInitialization_libKoBRA_DictOnly_Impl() {
    static const char* headers[] = {
"include/TPPACData.hxx",
"include/TLPPACData.hxx",
"include/TPlasticData.hxx",
"include/TSSDData.hxx",
"macros/kobra.hxx",
0
    };
    static const char* includePaths[] = {
"/home/ndpsdaq/packages/root/include",
"/home/ndpsdaq/online/cens/kobra",
"/home/ndpsdaq/packages/root/include/",
"/home/ndpsdaq/online/cens/kobra/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libKoBRA_DictOnly dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$include/TPPACData.hxx")))  TPPACData;
class __attribute__((annotate("$clingAutoload$include/TLPPACData.hxx")))  TLPPACData;
class __attribute__((annotate("$clingAutoload$include/TPlasticData.hxx")))  TPlasticData;
class __attribute__((annotate("$clingAutoload$include/TSSDData.hxx")))  TSSDData;
class __attribute__((annotate("$clingAutoload$macros/kobra.hxx")))  KOBRA;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libKoBRA_DictOnly dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "include/TPPACData.hxx"
#include "include/TLPPACData.hxx"
#include "include/TPlasticData.hxx"
#include "include/TSSDData.hxx"
#include "macros/kobra.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"KOBRA", payloadCode, "@",
"TLPPACData", payloadCode, "@",
"TPPACData", payloadCode, "@",
"TPlasticData", payloadCode, "@",
"TSSDData", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libKoBRA_DictOnly",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libKoBRA_DictOnly_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libKoBRA_DictOnly_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libKoBRA_DictOnly() {
  TriggerDictionaryInitialization_libKoBRA_DictOnly_Impl();
}
