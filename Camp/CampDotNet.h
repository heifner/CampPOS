// Copyright 2011 Kevin Heifner.  All rights reserved.
#ifndef CAMPDOTNET_H
#define CAMPDOTNET_H
#pragma once

class Camp;

using namespace System;
using namespace System::Collections::Generic;

public value struct CamperDotNet {
  Int64 id_;
  String^ firstName_;
  String^ lastName_;
  Single amount_;
};

// Wrap Camp API with a .NET class.
public ref class CampDotNet {
public:
	CampDotNet();
	~CampDotNet();

	void Start();
	void Shutdown();

	Int64 AddCamper(String^ firstName, String^ lastName, Single amount);

  /// Use finger print reader to find a camper.
  void GetCamper(Int64% id, String^% firstName, String^% lastName, Single% amount);

  IList<CamperDotNet>^ getAllCampers();

  void UpdateCamper(Int64 id, Single amount);
  void DeleteCamper(Int64 id);

private:
  // Can't put an unmanaged thing in a ref class, but a pointer to an unmanaged thing is okay.
  Camp* camp_;
};

public ref class CampException : public Exception {
public:
	CampException(String ^msg) : Exception(msg) {}
};

#endif // CAMPDOTNET_H
