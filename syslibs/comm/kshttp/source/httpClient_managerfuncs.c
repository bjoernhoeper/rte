/*
?*	Copyright (C) 2014
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************/
#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif

#include "config.h"

/*******************************************************************************************************************************************************************************
 * 				register
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_requestRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING servername,
		const OV_UINT	version,
		const OV_UINT port,
		const OV_UINT ttl,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_STRING requestUri = NULL;
	OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);

	/*	generate Header	*/
	ov_string_print(&requestUri, "/register?name=%s&port=%" OV_PRINT_UINT "&version=" OV_PRINT_UINT, servername, port, version);
	result = kshttp_generateAndSendHttpMessage("GET", "localhost", thisCl->v_ManagerPort, NULL, NULL, FALSE, requestUri, 0, NULL, Ov_PtrUpCast(kshttp_httpClientBase, thisCl), callbackThat, callback);
	return result;
}

/**
 *
 */
OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_processRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result
) {
	OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);
	OV_INSTPTR_ksbase_Channel pChannel = Ov_DynamicPtrCast(ksbase_Channel, Ov_GetFirstChild(ov_containment, thisCl));

	#if LOG_KS || LOG_KS_DEBUG
		OV_STRING strContent = NULL;
	#endif

	if(!thisCl || !pChannel ){
		*result = OV_ERR_BADPLACEMENT;
		return OV_ERR_BADPLACEMENT;
	}

	if(thisCl->v_httpStatusCode == 200){
		*result = OV_ERR_OK;
		KS_logfile_debug(("Registered at %s!", pChannel->v_address));
	}else if(thisCl->v_httpStatusCode == 406){
		*result = KS_ERR_NOMANAGER;
		KS_logfile_debug(("Not registered at %s! It is no Manager.", pChannel->v_address));
	}else{
		*result = KS_ERR_GENERIC;
		#if LOG_KS || LOG_KS_DEBUG
			strContent = Ov_HeapMalloc(thisCl->v_ServerResponse.contentLength+1);
			strncpy(strContent, (OV_STRING)pChannel->v_inData.readPT, thisCl->v_ServerResponse.contentLength);
			strContent[thisCl->v_ServerResponse.contentLength] = '\0';
			KS_logfile_debug(("Not registered at %s! Got http code: %" OV_PRINT_UINT ", with content: %s", pChannel->v_address, thisCl->v_httpStatusCode, strContent));
			Ov_HeapFree(strContent);
		#endif
	}

	return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				un-register
 ******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_requestUnRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		const OV_STRING servername,
		const OV_UINT version,
		const OV_INSTPTR_ov_domain	callbackThat,
		void (*callback)(const OV_INSTPTR_ov_domain this, const OV_INSTPTR_ov_domain that)
) {
	/*
	 *   local variables
	 */
	OV_RESULT result;
	OV_STRING requestUri = NULL;
	OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);

	/*	generate Header	*/
	ov_string_print(&requestUri, "/unregister?server=%s&version=%" OV_PRINT_UINT, servername, version);
	result = kshttp_generateAndSendHttpMessage("GET", "localhost", thisCl->v_ManagerPort, NULL, NULL, FALSE, requestUri, 0, NULL, Ov_PtrUpCast(kshttp_httpClientBase, thisCl), callbackThat, callback);
	return result;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_httpClient_processUnRegister(
		const OV_INSTPTR_ksbase_ClientBase this,
		const OV_INSTPTR_ksbase_ClientTicketGenerator TicketGenerator,
		OV_RESULT* result
) {
	/*
	 *   local variables
	 */

	OV_INSTPTR_kshttp_httpClient thisCl = Ov_StaticPtrCast(kshttp_httpClient, this);
#if LOG_KS || LOG_KS_DEBUG
	OV_INSTPTR_ksbase_Channel pChannel = Ov_DynamicPtrCast(ksbase_Channel, Ov_GetFirstChild(ov_containment, thisCl));
#endif

	if(thisCl->v_httpStatusCode == 200){
		*result = OV_ERR_OK;
		KS_logfile_debug(("unregistered at %s!", pChannel->v_address));
		return OV_ERR_OK;
	}else{
		KS_logfile_debug(("not unregistered at %s! Got http code: %" OV_PRINT_UINT " :-(", pChannel->v_address, thisCl->v_httpStatusCode));
		return OV_ERR_GENERIC;
	}

	return OV_ERR_OK;
}
