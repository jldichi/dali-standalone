#ifndef wx_fieldh
#define wx_fieldh

#ifdef __GNUG__
#pragma interface
#endif

#include <wx_item.h>
#include <wx_txt.h>

class wxField: public wxText
{
	typedef wxText inherited;

#ifdef wx_xview
	typedef Panel_setting (*xv_TextNotifyProc)(Panel_item, Event *);
	typedef void (*xv_TextEventProc)(Panel_item , Event *);
	xv_TextNotifyProc	xv_prevNotifyProc;
	xv_TextEventProc	xv_prevEventProc;
	static Panel_setting xv_NotifyProc(Panel_item item, Event * x_event);
	static void xv_EventProc(Panel_item item, Event * x_event);

	virtual void xv_OnChange();
#endif
#ifdef wx_motif
	static void m_modifyVerify(Widget w,
				XtPointer clientData, XtPointer callData);
	static void m_activate(Widget w,
				XtPointer clientData, XtPointer callData);
#endif
#ifdef wx_msw
	FARPROC oldWndProc;
	static LONG APIENTRY wxFieldSubClassWindowProc(HWND hWnd,
			UINT message, WPARAM wParam, LPARAM lParam);
	Bool sendIxFunctionKey(int key);
	virtual BOOL MSWCommand(UINT param, WORD id);
#endif

protected:
#ifdef wx_motif
	typedef XmTextVerifyCallbackStruct ModifyStruct;
#else
	struct TextBlock
	{
		char	*ptr;
		int		length;
		Bool	allocated;

		TextBlock() : ptr(""), length(0), allocated(FALSE) {}
		TextBlock(char *str) { tb_AllocText(str); }
		~TextBlock() { tb_FreeText(); }
		void SetNewText(const char *str) { tb_FreeText(); tb_AllocText(str); }

	private:
		void tb_FreeText() { if (allocated) delete [] ptr; }
		void tb_AllocText(const char *str)
			{ length = strlen(str); ptr = new char[length+1];
			  allocated = TRUE; memcpy(ptr,str,length+1); }
	};

	struct ModifyStruct
	{
		Bool	doit;
		int		currInsert, newInsert;
		int		startPos, endPos;
		int		startSel, endSel;
		TextBlock *text;

		ModifyStruct(wxField *item, TextBlock &tb) : doit(TRUE), text(&tb)
		{
#ifdef wx_msw
			DWORD sel = SendMessage((HWND)item->ms_handle,EM_GETSEL,0,0);
			currInsert = newInsert = startPos = startSel = LOWORD(sel);
			endPos = endSel = HIWORD(sel);
#else
			currInsert = newInsert = startPos = startSel = endPos = endSel = 0;
#endif
		}
	};
#endif

	static void SetNewText(ModifyStruct &ms, const char *str);
	virtual int GetTextLength() const;

	virtual void OnModifyVerify(ModifyStruct &ms);

#ifdef wx_msw
	void OnKeyDown(WPARAM wParam);
	void OnChar(WPARAM wParam);
	void OnClear(Bool copy);
	void OnPaste();
	void OnDelChar(Bool backspace);
	void FlushModify(ModifyStruct &ms);
#endif

public:
	wxField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL);

	virtual void ProcessCommand(wxCommandEvent & event);

	void SetValue(const char *value);

    void SetSkip(Bool skip = TRUE);
    void SetReadOnly(Bool readOnly = TRUE);
	void SetLength(int len);
};

inline void wxField::SetValue(const char *value)
	{ inherited::SetValue(value?(char*)value:""); }

////////////////////////////////////////////////////////////////////////////
//
// specific fields: ALPHA, NUM, DATE etc.
//
////////////////////////////////////////////////////////////////////////////

class wxCharField : public wxField
{
	int length_;
	char *testMask_;
	char *outputMask_;

protected:

	virtual void OnModifyVerify(ModifyStruct &ms);

public:
	wxCharField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL,
		int length = 1,
		const char *testMask = NULL, const char *outputMask = NULL);
	~wxCharField();
};

class wxNumField : public wxField
{

	int digits_, decimal_;
	Bool signed_;
	Bool useComma_;

#ifdef wx_xview
	virtual void xv_OnChange();
#endif
protected:

	virtual void OnModifyVerify(ModifyStruct &ms);

public:
	wxNumField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL,
		int digits = 1, int decimal = 0,
        Bool isSigned = FALSE, Bool useComma = FALSE);

};

class wxFloatField : public wxField
{
	int length_;

protected:

	virtual void OnModifyVerify(ModifyStruct &ms);

public:
	wxFloatField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL,
		int length = 6);

};

class wxBoolField : public wxField
{
	static char *yes_;
	static char *no_;
	static int length_;

protected:

	virtual void OnModifyVerify(ModifyStruct &ms);

public:
	wxBoolField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL,
		const char *yes = NULL, const char *no = NULL);

};

class wxDateTimeField : public wxField
{
	enum {
		MaxNum = 3,
		MaxLen = 10
	};
	char value_[MaxLen+1];

	int numbers_;
	int numPos_[MaxNum+1];
	int numLen_[MaxNum];

protected:

	int maxLength() const;

	virtual char delimiter() const = 0;
	virtual Bool validValue() = 0;

	int numbers() const;
	int number(int num) const;
	int minPossibleNumber(int num) const;
	int maxPossibleNumber(int num) const;

	Bool isDelimiterPos(int pos) const;
	Bool addNumber(int len);

protected:

	virtual void OnModifyVerify(ModifyStruct &ms);

public:
	wxDateTimeField(wxPanel *panel, wxFunction func, char *label,
		char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL);
};

inline int wxDateTimeField::numbers() const		{ return numbers_; }


class wxTimeField : public wxDateTimeField
{
protected:
	virtual char delimiter() const;

	static int maxValidNumber(int index);

	virtual Bool validValue();
public:
	wxTimeField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
        long style = 0, char *name = NULL, Bool haveSecond = TRUE);

};

class wxDateField : public wxDateTimeField
{
	enum NumType {
		Day,
		Month,
		Year
	};
	NumType numType[3];
	int numIndex[Year];
	Bool fullYear;

protected:
	virtual char delimiter() const;

	int maxValidNumber(int index);
	int minValidNumber(int index);

	virtual Bool validValue();
public:
	wxDateField(wxPanel *panel, wxFunction func, char *label, char *value = "",
		int x = -1, int y = -1, int width = -1, int height = -1,
		long style = 0, char *name = NULL,
        Bool haveYear = TRUE, Bool haveCentury = FALSE);

};

#endif // wx_fieldh
