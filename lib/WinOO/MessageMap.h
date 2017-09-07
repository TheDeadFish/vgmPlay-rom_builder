// implements a simple message map
#ifndef MESSAGE_MAP_H
#define MESSAGE_MAP_H

// single message testers
#define SINGLE_MSG_TST(_message) (message == _message)
#define WM_COMMAND_TST1(ctrlId) ((message == WM_COMMAND)&&( LOWORD(wParam) ==  ctrlId )&&(lParam != 0 ))
#define WM_COMMAND_TST2(ctrlId, nCode) ( WM_COMMAND_TST1(ctrlId)&&( HIWORD(wParam) == nCode ))
#define WM_DRAWITEM_TST(ctrlId) ((message == WM_DRAWITEM)&&(wParam == ctrlId))
// ctrlId message list testers
#define MESSAGE_MAP_TLST( ctrlId, ctrlLst ) (MessageMapArray(sizeof(ctrlLst)/sizeof(short),ctrlId,ctrlLst))
#define WM_COMMAND_TLST1( ctrlLst ) (((message==WM_COMMAND)&&(lParam!= 0))?MESSAGE_MAP_TLST(LOWORD(wParam),ctrlLst):(-1))
#define WM_COMMAND_TLST2( ctrlId, nCode) ((HIWORD(wParam)==nCode)?WM_COMMAND_TLST1( ctrlLst ):(-1))
#define WM_DRAWITEM_TLST( ctrlLst ) ((message==WM_DRAWITEM)?MESSAGE_MAP_TLST(wParam,ctrlLst):(-1))
// messages for single entry, single message
#define MESSAGE_MAP_MSG( func, fish ) { if( fish ) { func( hwnd, wParam, lParam ); MessageHandled |= true; }}		
#define SINGLE_MSG( func, _message ) MESSAGE_MAP_MSG( func, SINGLE_MSG_TST( _message )
#define WM_COMMAND_MSG( func, ctrlId ) MESSAGE_MAP_MSG( func, WM_COMMAND_TST1(ctrlId) )
#define WM_COMMAND_MSG2( func, ctrlId, nCode) MESSAGE_MAP_MSG( func, WM_COMMAND_TST2( ctrlId, nCode ) )
#define WM_DRAWITEM_MSG( func, ctrlId ) MESSAGE_MAP_MSG( func, WM_DRAWITEM_TST( ctrlId ) )
// messages for single entry, list messages
#define MESSAGE_MAP_LST( func, fish ) { int fish2 = fish; if( fish2 != -1){ func( fish2, hwnd, wParam, lParam ); MessageHandled |= true; }}
#define WM_COMMAND_LST( func, ctrlLst ) MESSAGE_MAP_LST( func, WM_COMMAND_TLST1( ctrlLst ) )
#define WM_COMMAND_LST2( func, ctrlLst, nCode) MESSAGE_MAP_LST( func, WM_COMMAND_TLST2( ctrlLst, nCode) )
#define WM_DRAWITEM_LST( func, ctrlLst ) MESSAGE_MAP_LST( func, WM_DRAWITEM_TLST( ctrlLst ) )
// messages for multi entry, single message
#define ARRAYMSG_MSG( fish ) if ( fish ) continue; num++;
#define SINGLE_MESSAGEM( _message ) ARRAYMSG_MSG( SINGLE_MSG_TST( _message )
#define WM_COMMAND_MSGM( ctrlId ) ARRAYMSG_MSG( WM_COMMAND_TST1(ctrlId) )
#define WM_COMMAND_MSG2M( ctrlId, nCode) ARRAYMSG_MSG( WM_COMMAND_TST2( ctrlId, nCode ) )
#define WM_DRAWITEM_MSGM( ctrlId ) ARRAYMSG_MSG( WM_DRAWITEM_TST( ctrlId ) )
// messages for multi entry, list messages
#define ARRAYMSG_LST( ctrlLst, fish ) { int fish2 = fish; if( fish2 != -1 ) { num+= fish2; continue; } num += sizeof)ctrlLst/sizeof(short);}
#define WM_COMMAND_LSTM( ctrlLst ) ARRAYMSG_LST( ctrlLst, WM_COMMAND_TLST1( ctrlLst ) )
#define WM_COMMAND_LST2M( ctrlLst, nCode) ARRAYMSG_LST( ctrlLst, WM_COMMAND_TLST2( ctrlLst, nCode) )
#define WM_DRAWITEM_LSTM( ctrlLst ) ARRAYMSG_LST( ctrlLst, WM_DRAWITEM_TLST( ctrlLst ) )

#define MESSAGE_MAP_ALL(func) 												\
	MessageHandled |= func( hwnd, message, wParam, lParam );

#define MESSAGE_MAP() 														\
bool MessageMap(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lresult){ 	\
	bool MessageHandled = false;
	
#define _MESSAGE_MAP()														\
	return MessageHandled;}

#define MULTIMSG_MAP( func ) {			\
	int num = 0;					\
	goto forSkip;					\
	for(;;){					\
		func(num, hwnd, wParam, lParam);	\
		MessageHandled |= true;			\
		break;							\
	forSkip:

#define _MULTIMSG_MAP() break;}}

#define MSGIF( fish ) if( fish ) if( MessageHandled |= true)
#define MSGTRUE() MessageHandled |= true;

// CtrlId array processing stuff
static
int MessageMapArray( int count, short ctrlId, short *ctrlList )
{
	while((--count) >= 0)
		if(ctrlId == ctrlList[count])
			break;
	return count;
}

// MFC message map recreation
#define DECLARE_MESSAGE_MAP() \
long _MessageMap_(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);	\
long MessageMap(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){		\
	_MessageMap_(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);}
	
#define BEGIN_MESSAGE_MAP( class ) \
long class::_MessageMap_(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){	\
	switch(message){

#define END_MESSAGE_MAP() default: return 0; }} 

// MFC message list
//#define ON_MESSAGE(msg, func) case msg: return func(wParam, lParam);
#define ON_WM_LBUTTONDOWN() case WM_LBUTTONDOWN:{ func(wParam, CPoint(lParam)); return 1;}
#define ON_WM_LBUTTONDBLCLK() case WM_LBUTTONDOWN:{ func(wParam, CPoint(lParam)); return 1;}
#define ON_WM_KEYDOWN() case WM_KEYDOWN: { func(wParam, LOWORD(lParam), HIWORD(lParam)); return 1;}
// more to come !

#endif
