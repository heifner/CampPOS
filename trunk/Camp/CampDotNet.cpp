// Copyright 2011 Kevin Heifner.  All rights reserved.

#include "CampDotNet.h"
#include "Camp.h"
#include <vcclr.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace {
  std::string toString(String^ str) {
    char* ansiStr = static_cast<char*>(Marshal::StringToHGlobalAnsi(str).ToPointer());
    std::string result = ansiStr;
    Marshal::FreeHGlobal(static_cast<IntPtr>(ansiStr));
    return result;
  }
}

CampDotNet::CampDotNet() 
: camp_(new Camp())
{}

CampDotNet::~CampDotNet() 
{
	delete camp_;
}

void 
CampDotNet::Start() 
{
  try {
    camp_->start();
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

void
CampDotNet::Shutdown() 
{
  try {
    camp_->stop();
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

Int64 
CampDotNet::AddCamper(String^ firstName, String^ lastName, Single amount)
{
  try {
    std::string cppFirstName = toString(firstName);
    std::string cppLastName = toString(lastName);
    float cppAmount = amount;
    FPKey id = camp_->addCamper(cppFirstName, cppLastName, cppAmount);
    return id;
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}


CamperDotNet
CampDotNet::GetCamper(Int64 id) 
{
  try {
    ref_ptr<Camper> camper = camp_->getCamper(id);

    CamperDotNet camperDotNet;
    camperDotNet.id_ = camper->getId();
    camperDotNet.firstName_ = gcnew String(camper->getFirstName().c_str());
    camperDotNet.lastName_ = gcnew String(camper->getLastName().c_str());
    camperDotNet.amount_ = camper->getAmount();

    return camperDotNet;
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

Int64
CampDotNet::FindCamper()
{
  try {
    FPKey key = camp_->findCamper();
    return key;
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

IList<CamperDotNet>^ 
CampDotNet::getAllCampers()
{
  try {
    List<CamperDotNet>^ result = gcnew List<CamperDotNet>();
    for (Camp::FPKeyToCamperMap::const_iterator i = camp_->begin(), end = camp_->end(); i != end; ++i) {
      ref_ptr<Camper> camper = i->second;
      CamperDotNet camperDotNet;
      camperDotNet.id_ = camper->getId();
      camperDotNet.firstName_ = gcnew String(camper->getFirstName().c_str());
      camperDotNet.lastName_ = gcnew String(camper->getLastName().c_str());
      camperDotNet.amount_ = camper->getAmount();
      result->Add(camperDotNet);
    }
    return result;
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

void 
CampDotNet::UpdateCamper(Int64 id, Single amount)
{
  try {
    FPKey key = id;
    float cppAmount = Convert::ToSingle(amount);
    
    ref_ptr<Camper> camper = camp_->getCamper(key);
    camper->setAmount(cppAmount);
    camp_->updateAmount(id, cppAmount);
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

void 
CampDotNet::DeleteCamper(Int64 id)
{
  try {
    FPKey key = id;
    camp_->deleteCamper(key);
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}

void
CampDotNet::CancelOperation()
{
  try {
    camp_->cancelOperation();
  } catch (std::exception& e) {
		throw gcnew CampException(gcnew String(e.what()));
  }
}
