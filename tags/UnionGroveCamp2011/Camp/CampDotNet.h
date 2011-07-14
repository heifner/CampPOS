// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef CAMPDOTNET_H
#define CAMPDOTNET_H
#pragma once

class Camp;

using namespace System;
using namespace System::Collections::Generic;

/// .Net Interfaces.

/// .Net struct for Camper.
/// @see Camper
public value struct CamperDotNet {
  Int64 id_;
  String^ firstName_;
  String^ lastName_;
  Single amount_;
};

/// .Net wrapper for Camp API.
/// @see Camp for documentation. 
public ref class CampDotNet {
public:
	CampDotNet();
	~CampDotNet();

	void Start();
	void Shutdown();

	Int64 AddCamper(String^ firstName, String^ lastName, Single amount);

  CamperDotNet GetCamper(Int64 id);

  Int64 FindCamper();

  IList<CamperDotNet>^ getAllCampers();

  void UpdateCamper(Int64 id, Single amount);

  void DeleteCamper(Int64 id);

  void CancelOperation();

  Single GetDefaultPrice();

private:
  // Can't put an unmanaged thing in a ref class, but a pointer to an unmanaged thing is okay.
  Camp* camp_;
};

/// .Net exception to wrap std::exception
public ref class CampException : public Exception {
public:
	CampException(String ^msg) : Exception(msg) {}
};

#endif // CAMPDOTNET_H
