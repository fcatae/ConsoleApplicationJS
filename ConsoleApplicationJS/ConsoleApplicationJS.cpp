// ConsoleApplicationJS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define CHECKERROR(err)		if(err!=JsNoError) throw;

void __stdcall beforeCollectCallback(void* state)
{
	printf("GC");

}

JsValueRef __stdcall EchoCallback(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
{
	printf("Hello!");

	return nullptr;
}

int main()
{
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsErrorCode error;

	error = JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);
	CHECKERROR(error);

	error = JsCreateContext(runtime, &context);
	CHECKERROR(error);

	error = JsSetCurrentContext(context);
	CHECKERROR(error);

	JsValueRef numberOne;
	JsIntToNumber(1, &numberOne);

	JsValueRef global;
	error = JsGetGlobalObject(&global);
	
	JsValueRef functionName;
	JsValueRef functionExternal;
	error = JsPointerToString(L"run", 4, &functionName);
	error = JsCreateNamedFunction(functionName, EchoCallback, nullptr, &functionExternal);

	JsPropertyIdRef propEcho;
	error = JsGetPropertyIdFromName(L"run", &propEcho);
	error = JsSetProperty(global, propEcho, functionExternal, true);

	JsValueRef script;
	unsigned int sourceContext = 0;
	error = JsRunScript(L"var a; run();", sourceContext, L"", &script);
	
	// handle syntax error
	
	CHECKERROR(error);

	JsValueRef symbol;
	JsValueRef symbolName;
	error = JsPointerToString(L"s", 1, &symbolName);
	error = JsCreateSymbol(symbolName, &symbol);

	JsValueRef funcSum;
	error = JsParseScript(L"run();", 1, L"sum.js", &funcSum);
	CHECKERROR(error);

	JsValueRef nullValue;
	error = JsGetNullValue(&nullValue);
	CHECKERROR(error);

	JsValueRef params;
	error = JsCreateArray(1, &params);
	JsValueRef one;
	error = JsIntToNumber(0, &one);
	error = JsSetIndexedProperty(params, one, nullValue);
	
	JsSetRuntimeBeforeCollectCallback(runtime, nullptr, beforeCollectCallback);
	JsCollectGarbage(runtime);

	JsValueRef result;
	error = JsCallFunction(funcSum, &global, 1, &result);
	CHECKERROR(error);

	// check failure

	//CHECKERROR(error);

	//JsCreateNamedFunction()

	return 0;
}

