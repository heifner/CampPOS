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

void 
CampDotNet::GetCamper(Int64% id, String^% firstName, String^% lastName, Single% amount)
{
  try {
    FPKey key = camp_->findCamper();
    ref_ptr<Camper> camper = camp_->getCamper(key);

    id = key;
    firstName = gcnew String(camper->getFirstName().c_str());
    lastName = gcnew String(camper->getLastName().c_str());
    amount = camper->getAmount();
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


#ifdef JUNK

void DataServerConnector::Run() {
	int argc = 0;
	wchar_t **argv = NULL;

	try {
		// convert .NET arguments to standard argc/argv
		array<String^>^ arguments = Environment::GetCommandLineArgs();
		argc = arguments->Length;
		argv = new wchar_t *[argc];
		for (int i=0; i<argc; i++) {
			pin_ptr<const wchar_t> arg = PtrToStringChars(arguments[i]);
			argv[i] = _wcsdup(arg);
		}

		// initialize the ORB
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		// USE ORBINITREF - start server with -ORBListenEndpoints iiop://machine:port
		// and client with -ORBInitRef DataServer=corbaloc:iiop:machine:port/DataServer
		CORBA::Object_var database_obj = orb->resolve_initial_references("DataServer");
		if (CORBA::is_nil(database_obj.in())) 
			throw std::exception("Could not get the Database IOR");

		// narrow the IOR to a Database object reference.
		Database_var database = Database::_narrow(database_obj.in());
		if (CORBA::is_nil(database.in()))
			throw std::exception("IOR was not a Database object reference");

		// save the references via a pointer to an unmanaged class
		state_ = new DataServerConnectorState(orb, database);

		// good to go - tell the outside world
		startupEvent_.Set();

		// run the ORB - need to wait anyway for DDS, so may as well process AMI calls, etc.
		orb->run();
		orb->destroy();
	}
	catch (CORBA::Exception& ex) {
		std::stringstream ss;
		ss << "Exception: " << ex;
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
	catch (std::exception& ex) {
		std::stringstream ss;
		ss << "Exception: " << ex.what();
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
}


// *** .NET wrappers for CORBA methods ***
// DataConnector must have a pure .NET interface, so arguments must be CLR types

bool DataServerConnector::CreateItem(String ^description, Int64 %id) {
	try {
		pin_ptr<const wchar_t> cppDescription = PtrToStringChars(description);
		CORBA::WString_var desc = CORBA::wstring_dup(cppDescription);
		CORBA::LongLong cid;
		CORBA::Boolean result = state_->DatabasePtr()->CreateItem(desc, cid);
		id = cid;
		return result;
	} catch (CORBA::Exception &ex) {
		std::stringstream ss;
		ss << "Exception: " << ex;
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
}

bool DataServerConnector::ReadItem(Int64 id, String^% description) {
	try {
		CORBA::WString_var desc;
		CORBA::Boolean result = state_->DatabasePtr()->ReadItem(id, desc);
		if (result)
			description = gcnew String(desc.in());
		return result;
	} catch (CORBA::Exception &ex) {
		std::stringstream ss;
		ss << "Exception: " << ex;
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
}

bool DataServerConnector::UpdateItem(Int64 id, String^ description) {
	try {
		pin_ptr<const wchar_t> cppDescription = PtrToStringChars(description);
		CORBA::WString_var desc = CORBA::wstring_dup(cppDescription);
		return state_->DatabasePtr()->UpdateItem(id, desc);
	} catch (CORBA::Exception &ex) {
		std::stringstream ss;
		ss << "Exception: " << ex;
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
}

bool DataServerConnector::DeleteItem(Int64 id) {
	try {
		return state_->DatabasePtr()->DeleteItem(id);
	} catch (CORBA::Exception &ex) {
		std::stringstream ss;
		ss << "Exception: " << ex;
		throw gcnew DataConnectorException(gcnew String(ss.str().c_str()));
	}
}

#endif