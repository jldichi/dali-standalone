#pragma interface

#include "common.h"
#include <wx_check.h>
#include <wx_lbox.h>
#include <wx_rbox.h>
#include <wx_slidr.h>

class wxCheckBoxField: public wxCheckBox
{
	typedef wxCheckBox inherited;

	enum CheckValue
	{
		c_null = -1,
		c_false,
		c_true
	};
 	CheckValue value_;
 	Bool readOnly_;

 	wxFunction callBack;
	static void actionFunc(wxObject& item, wxEvent& event);

	void _SetValue(CheckValue theValue);
	void UpdateValue();

#ifdef wx_msw
	FARPROC oldWndProc;
	static LONG APIENTRY wxFieldSubClassWindowProc(HWND hWnd,
			UINT message, WPARAM wParam, LPARAM lParam);
#endif

public:
	wxCheckBoxField(wxPanel *panel, wxFunction func,
  		int x, int y, int width, int height, long style, char *name);
  	~wxCheckBoxField(void);
  	void SetSkip(Bool skip);
  	void SetReadOnly(Bool readOnly);
	void SetValue(Bool theValue);
	void ClearValue();
	Bool GetValue() const;
	Bool IsCleared() const;
};

inline void wxCheckBoxField::SetValue(Bool v)	{_SetValue(v?c_true:c_false);}
inline void wxCheckBoxField::ClearValue()		{_SetValue(c_null);}
inline Bool wxCheckBoxField::GetValue()	const	{return value_==c_true;}
inline Bool wxCheckBoxField::IsCleared() const 	{return value_==c_null;}

class wxListBoxField: public wxListBox
{
	typedef wxListBox inherited;

 	int selection_;
 	Bool readOnly_;
 	Bool notNull_;

 	wxFunction callBack;
	static void actionFunc(wxObject& item, wxEvent& event);

	void UpdateValue();

#ifdef wx_msw
	FARPROC oldWndProc;
	static LONG APIENTRY wxFieldSubClassWindowProc(HWND hWnd,
			UINT message, WPARAM wParam, LPARAM lParam);
	Bool sendIxFunctionKey(int key);
#endif

public:
	wxListBoxField(wxPanel *panel, wxFunction func,
  		int x, int y, int width, int height,
  		int N, char **choices, long style, char *name);
	~wxListBoxField();
	void SetSkip(Bool skip);
	void SetReadOnly(Bool readOnly);
	Bool SetStringSelection(char *s);
	void SetSelection(int N);
	int GetSelection();
	void SetNotNull(Bool notNull);
};

inline int wxListBoxField::GetSelection()		{return selection_;}

class wxRadioBoxField: public wxRadioBox
{
	typedef wxRadioBox inherited;

 	int selection_;
 	int lastSelected_;
 	Bool readOnly_;
 	Bool notNull_;

 	wxFunction callBack;

	static void actionFunc(wxObject& item, wxEvent& event);

	void UpdateValue();

#ifdef wx_msw
	FARPROC oldWndProc;
	static LONG APIENTRY wxFieldSubClassWindowProc(HWND hWnd,
			UINT message, WPARAM wParam, LPARAM lParam);
	Bool sendIxFunctionKey(int key);

	virtual BOOL MSWCommand(UINT param, WORD id);
#endif

public:
    wxRadioBoxField(wxPanel *panel, wxFunction func,
  		int x, int y, int width, int height,
  		int n, char **choices, long style, char *name);
  	~wxRadioBoxField();
  	Bool SetStringSelection(char *s);
  	void SetSelection(int N);
  	int GetSelection();
  	virtual void SetFocus();
  	void SetNotNull(Bool notNull);
  	void SetSkip(Bool skip);
  	void SetReadOnly(Bool readOnly);
};

inline int wxRadioBoxField::GetSelection()			{return selection_;}

class wxSliderField: public wxSlider
{
	typedef wxSlider inherited;

	long value_;
	long mult;
	Bool readOnly_;

	wxFunction callBack;
	static void actionFunc(wxObject& item, wxEvent& event);

	long ItemToField(int itemValue) const;
	int FieldToItem(long fieldValue) const;
	void UpdateValue();

#ifdef wx_msw
	FARPROC oldWndProc;
	static LONG APIENTRY wxFieldSubClassWindowProc(HWND hWnd,
			UINT message, WPARAM wParam, LPARAM lParam);
	Bool sendIxFunctionKey(int key);
#endif

public:
	wxSliderField(wxPanel *panel, wxFunction func,
		long min_value, long max_value,
		int x, int y, int width, int height, long style, char *name, int decimal);
	Bool Create (wxPanel * panel, wxFunction func, long value,
		long min_value, long max_value, int x, int y, int width, int height,
		long style, char *name);
	~wxSliderField(void);
	void SetSkip(Bool skip);
	void SetReadOnly(Bool readOnly);
	void SetFieldValue(long value);
	long GetFieldValue(void);
	virtual void SetSize(int x, int y, int width, int height);
	virtual void SetFocus();
};

