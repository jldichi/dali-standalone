/********************************************************************
* Copyright (c) 1994 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: fmtbar.h,v 1.1.1.1 1996/04/16 18:51:41 gustavof Exp $
*
* DESCRIPTION
*********************************************************************/

#ifndef WXFMTBAR_H
#define WXFMTBAR_H

#ifdef __GNUG__                                                                 
#pragma interface                                                              
#endif

class FmToolBarInt;
class FormRep;
class wxColour;
class wxBitmap;
class FmToolbar;
typedef wxBitmap FmBitmap;

struct ToolbarButton {
	Int actionId_;
	String bitmapName_;
	String pushedBitmapName_;
	bool toggle_;
	Int	xPos_;
	Int yPos_;
	FmToolbar *parent_;
	ToolbarButton():
       	actionId_(Fm::no_action), bitmapName_(NULL_STRING),                          
        pushedBitmapName_(NULL_STRING), toggle_(false), 
	xPos_(-1), yPos_(-1) {};
	ToolbarButton(Int actionId, const String& bitmapName = NULL_STRING, 
			   const String& pushedBitmapName = NULL_STRING,
               bool toggle = false, Int xPos = -1, Int yPos = -1):
    	actionId_(actionId), bitmapName_(bitmapName),
    	pushedBitmapName_(pushedBitmapName), toggle_(toggle),
    	xPos_(xPos), yPos_(yPos) {};
    
    inline void parent(FmToolbar *theParent); 
    void store(FILE *out);
    bool load(FILE *in);	           
};

class gtype(PtrArray, ToolbarButton); 

class FmToolbar: public FmObject {
    typedef FmObject inherited;  

	gtype(PtrArray, ToolbarButton)   &toolbarButtons;     
    Int orientation_;
    Int nRowsOrColumns_;                    
    FmToolBarInt * toolbar;  
        
public:
  	FmToolbar();
  	~FmToolbar();             

	FM_DECLARE_CLASS();

	void initValues();
    void orientation(Int orientation);
    inline Int orientation() const;
    void nRowsOrColumns(Int nRowsOrColumns);
    ToolbarButton &toolbarButton(Int i) const;
    void getSize(Int &w, Int &h);
    void setSize(Int x, Int y, Int w, Int h);
                       	
    void addToolbarButton(ToolbarButton * but); 
  	void addToolbarButton(Int actionId, 
      			const String& bitmapName = NULL_STRING,
 			   	const String& pushedBitmapName = NULL_STRING,
               	bool toggle = false, Int xPos = -1, Int yPos = -1);

    void addSeparator(); 

    virtual void _realize();        // create widgets this only             
    virtual void _unrealize();      // destroy windgets this only
};

#include <ideafix/priv/fmtbar.icc>

#endif

     
