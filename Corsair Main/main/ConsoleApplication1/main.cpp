#include "CUESDK.h"

#include "windows.h"
#include <iostream>
#include <thread>
#include <future>
#include <vector>

const char* toString(CorsairError error) {
	switch (error)
	{
		switch (error) {
		case CE_Success:
			return "CE_Success";
		case CE_ServerNotFound:
			return "CE_ServerNotFound";
		case CE_NoControl:
			return "CE_NoControl";
		case CE_ProtocolHandshakeMissing:
			return "CE_ProtocolHandshakeMissing";
		case CE_IncompatibleProtocol:
			return "CE_IncompatibleProtocol";
		case CE_InvalidArguments:
			return "CE_InvalidArguments";
		default:
			return "unknown error";
		}
}