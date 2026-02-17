/********************************************************************
* Copyright (c) 1991 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: dalirc.cc,v 1.3 1997/11/18 19:23:53 guiller Exp $
*
* DESCRIPTION: Implementation of dalirc class
*********************************************************************/

#ifdef __GNUC__
#pragma implementation
#endif

#include <ifound.h>
#include <dalirc.h>
#include <ifound/str.h>
#include <editor.h>                         
#include <local/strfile.h>
#include <winapp/window.h>
#include <winapp/msgbox.h>
#include <ifound/stdc.h>

static struct tokenTab {
	const char *lexeme;
	RcToken token;
} table[] = {
	// OJO! La lista debe estar ordenada alfabeticamente!
	{ "autosavefreq",			AUTOSAVEFREQ  	},
  	{ "buildcmd",				BUILDCOMMAND	},
  	{ "buildflags",				BUILDFLAGS		},
	{ "commandmode",			COMMANDMODE  	},
	{ "daliman",				DALIMAN      	},
	{ "dalitags",				DALITAGS     	},
	{ "desktopchar",			DESKTOPCHAR  	},
	{ "diffcmd",				DIFFCMD			},
	{ "footerheight",			FOOTERHEIGHT  	},
	{ "funcexpbrowser",			FUNCEXPBROWSER	},
	{ "functionsensor",			FUNCTIONSENSOR	},
  	{ "historysize",			HISTORYSIZE  	},
	{ "matchtab",				MATCHTAB     	},
	{ "maxviews",				MAXVIEWS     	},
    { "savedesktop",			SAVEDESKTOP  	},
	{ "scratchviews",			SCRATCHVIEWS	},
	{ "searchpath",				SEARCHPATH      },
	{ "sourcebrowser",			SOURCEBROWSER   },
	{ "tabsize",				TABSIZE     	},
	{ "usermode",				USERMODE     	},
	{ "windowstile",			WINDOWSTILE 	}
};


static Int tabsize = (sizeof(table)/sizeof(table[0]));

// This function is being called from an extern "C" function, so it
// should also be extern "C" (could have problems in Zortech).
extern "C" {

	static int cmpTokens(char *s1, char *s2)
	{
		return strcmp(((tokenTab*) s1)->lexeme, ((tokenTab*) s2)->lexeme);
	}

}

/*
 *		Class DaliRc
 */
DaliRc::DaliRc(const String &file) : fileName(file), ifile(file)
{
	idStart();
}

DaliRc::~DaliRc()
{                      
	idCheck();
	idStop();
}

bool DaliRc::readFile()
{               
	idCheck();
	bool flag;
	String line;

	while (ifile >> line) {
		if (line.length() == 0 || line[0] == '#')
			continue;

		String lvalue = line.field(0, "=");
		lvalue.rTrim();

		String rvalue = line.field(1, "=");
		rvalue.lTrim();
        
		switch (findToken(lvalue)) {
			case AUTOSAVEFREQ:                                                          
				Editor::autoSaveFreq(toInt(rvalue));
				break;                     

			case COMMANDMODE:
			    flag = (rvalue.toLower() == "on") ? true : false; 	
				Editor::setCmdLine(flag);
				break;                     

			case DALIMAN:
				Editor::daliMan = rvalue;
				break;

			case DIFFCMD:
				Editor::diffCmd = rvalue;
				break;

			case BUILDCOMMAND:
				Editor::buildCmd = rvalue;
				break;

			case BUILDFLAGS:
				Editor::buildFlags = rvalue;
				break;
				
			case DALITAGS:
			   	if (Editor::daliTags != NULL_STRING)   
					Editor::daliTags += ":";
			   	Editor::daliTags += rvalue.expandEnvVars();
				break;				
				
        	case DESKTOPCHAR:         
        		Editor::deskTopChar = (rvalue.toLower() == "shadow") ? 
						true : false;
        		break;

			case FOOTERHEIGHT:
				Editor::footerHeight = toInt(rvalue);
				break;				

			case FUNCTIONSENSOR:
        		Editor::functionSensor = (rvalue.toLower() == "on") ? 
						FS_ON : FS_OFF;
				break;

			case FUNCEXPBROWSER:
        		Editor::funcExpBrowser = (rvalue.toLower() == "on") ? 
						FS_ON : FS_OFF;
				break;

			case HISTORYSIZE:
			    {
					Int rv = toInt(rvalue);
					if (rv > 0)
						Editor::historySize(rv);
				}
				break;

			case MATCHTAB:  
			{
			    for ( StrCursor mt(rvalue,":") ; mt ; mt++ ) {
			           String matchPair = mt.value();
			           Editor::addMatchPair(matchPair.field(0," "),
			           					 	matchPair.field(1," ")
			           					   );     
				}
				break;				
            }
            
			case MAXVIEWS:
				Editor::maxViews = toInt(rvalue);
				break;

        	case SAVEDESKTOP:         
        		Editor::saveDesktop = (rvalue.toLower() == "on") ? 
						true : false;
        		break;

			case SCRATCHVIEWS:
				Editor::scratchViews = toInt(rvalue) > 0 ? toInt(rvalue) : 1;
				break;

			case SEARCHPATH:
				if (Editor::searchPath != NULL_STRING)
					Editor::searchPath += ":";
				Editor::searchPath += rvalue;
				break;				

			case SOURCEBROWSER:
				Editor::addSourceBrowserEntry(
						rvalue.field(0,":"),
						rvalue.field(2,":"),
						rvalue.field(3,":"),
						rvalue.field(4,":"),
						rvalue.field(5,":"),
						rvalue.field(1,":"));
				break;

			case TABSIZE:
				{
					Int rv = toInt(rvalue);
					if (rv > 0)
						Editor::tabSize(toInt(rvalue));
				}
				break;

			case USERMODE:

				Editor::userMode = rvalue.toLower();
				break;				

			case WINDOWSTILE:
				{
					Int rv = toInt(rvalue);
					if (rv > 0)
						Editor::winsTile = rv;
				}
				break;    

			case ERR:
				 {  
				 	MsgBox errbox(libMsg("WARNING"),String("Invalid token in .dalirc (")+lvalue+"="+rvalue+")",
				 				   9, 35, 5, 20);
					errbox.execute();
				 }
				 break;           
			default:
			    assert(0);
		}		
	}

	return true;
}

RcToken DaliRc::findToken(String &lexeme)
{
	idCheck();
	static tokenTab elem;
	String tmp = lexeme.toLower();
	elem.lexeme = toCharPtr(tmp);
	
	Int i = locate((const char *)&elem, (const char *)table, 	
			   UnInt(tabsize), sizeof(table[0]), IFPCPCP(cmpTokens));
	if (i < 0)
		return RcToken(ERR);

	return table[i].token;
}


