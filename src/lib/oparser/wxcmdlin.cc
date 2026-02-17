// --------------------------------------------------------------------------
// Copyright (c) 1995 InterSoft Co.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: wxcmdlin.cc,v 1.1.1.1 1996/04/16 18:52:11 gustavof Exp $
//
// Description:
// Implementation of wxWindows option parsing class
// --------------------------------------------------------------------------

#include <ifound/oparser.h>
#include <ifound/stdc.h>
#include <ifound/parray.h>

declare(PtrArray, Option);

#if 0
#include <wxwin/all.h>

#include <ifound.h>

#include <ifound/wcmdline.h>
#include <ifound/stdc.h>

declare(PtrArray, Option);

// Option class

static WinOptionParser *op = NULL;

#define VAL_SEPARATOR	"\t"

void StrOption::createItem(wxPanel *thePanel)
{
	if (values.isNull()) {
		text_ = tr_new wxText(thePanel, NULL, toCharPtr(description()));
		text_->SetValue(toCharPtr(str));
	} else {
		char *choices[20];
		int n = 0;
		for (StrCursor sc(values, VAL_SEPARATOR); sc; ++sc) {
			String tmpStr = sc.value();
			choices[n] = tr_new char [tmpStr.length() + 1];
			strcpy(choices[n], toCharPtr(tmpStr));
			++n;
		}
		rbox_ = tr_new wxRadioBox(thePanel, NULL,
				toCharPtr(description()), -1, -1, -1, -1,
				n, choices);
		rbox_->SetSelection(val);
	}
}

void StrOption::assignItem()
{
	if (values.isNull()) {
		assert(text_ != NULL);
		str = text_->GetValue();
	} else {
		assert(rbox_ != NULL);
		val = rbox_->GetSelection();
		str = values.field(val, VAL_SEPARATOR);
	}
}

void NumOption::createItem(wxPanel *thePanel)
{
	text_ = tr_new wxText(thePanel, NULL, toCharPtr(description()));
	String tmp = toString(n);
	text_->SetValue(toCharPtr(tmp));
}

void NumOption::assignItem()
{
	assert(text_ != NULL);
	n = toInt(text_->GetValue());
}

void BoolOption::createItem(wxPanel *thePanel)
{
	checkbox_ = tr_new wxCheckBox(thePanel, NULL,
			toCharPtr(description()));
	checkbox_->SetValue((b == true) ? true : false);
}

void BoolOption::assignItem()
{
	assert(checkbox_ != NULL);
	b = checkbox_->GetValue();
}

void okProc(wxButton& but, wxCommandEvent& event)
{
	op->assign();
	wxDialogBox *dialog = (wxDialogBox *) but.GetParent();
	dialog->Show(FALSE);
}

void cancelProc(wxButton& but, wxCommandEvent& event)
{
	wxDialogBox *dialog = (wxDialogBox *) but.GetParent();
	dialog->Show(FALSE);
}

void addProc(wxButton& but, wxCommandEvent& event)
{
	char *f = wxFileSelector("Select File", NULL, NULL, NULL, "*.fm");
	if (f != NULL) {
		op->listBox()->Append(f);
	}
}

void delProc(wxButton& but, wxCommandEvent& event)
{
	op->listBox()->Delete(op->listBox()->GetSelection());
}

WinOptionParser::WinOptionParser(const String &vers)
: OptionParser(vers)
{
}

WinOptionParser::~WinOptionParser()
{
}

bool WinOptionParser::execute()
{
	Int nOpts;
	Int j;

	String appNm = appName();
	wxDialogBox *dbox_ = tr_new wxDialogBox(NULL, toCharPtr(appNm), TRUE);

	String pName = argVal(0);
	// tr_new wxMessage(dbox_, toCharPtr(pName));
	// dbox_->NewLine();

	nOpts = opts.dim();

	for (j = 0; j < nOpts; j++) {
		opts.elem(j)->createItem(dbox_);
		dbox_->NewLine();
	}

#if 0
	if (1) {
		listbox_ = tr_new wxListBox(dbox_, NULL, "Files", wxSINGLE, -1, -1,
				500, 100, 0, NULL);
		tr_new wxButton(dbox_, wxFunction(addProc), "Add");
		tr_new wxButton(dbox_, wxFunction(delProc), "Del");
		dbox_->NewLine();
	}
#endif

	tr_new wxButton(dbox_, wxFunction(okProc), "OK");
	tr_new wxButton(dbox_, wxFunction(cancelProc), "Cancel");
	dbox_->NewLine();
	dbox_->Fit();

	op = this;

	dbox_->Show(TRUE);
	return true;
}

void OptionParser::assign()
{
	Int nOpts = opts.dim();
	Int j;

	for (j = 0; j < nOpts; j++) {
		opts.elem(j)->assignItem();
	}
}
#else

void StrOption::createItem(wxPanel *)
{
}

void StrOption::assignItem()
{
}

void NumOption::createItem(wxPanel *)
{
}

void NumOption::assignItem()
{
}

void BoolOption::createItem(wxPanel *)
{
}

void BoolOption::assignItem()
{
}

void OptionParser::assign()
{
	Int nOpts = opts.dim();
	Int j;

	for (j = 0; j < nOpts; j++) {
		opts.elem(j)->assignItem();
	}
}
#endif
