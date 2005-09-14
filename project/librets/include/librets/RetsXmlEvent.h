/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
#ifndef LIBRETS_RETS_XML_EVENT_H
#define LIBRETS_RETS_XML_EVENT_H

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class RetsXmlEvent : public RetsObject
{
  public:
    enum Type {
        START_ELEMENT,
        END_ELEMENT,
        TEXT,
        END_DOCUMENT,
    };
    
    RetsXmlEvent();
    RetsXmlEvent(int lineNumber, int columnNumber);
    
    virtual ~RetsXmlEvent();

    virtual Type GetType() const = 0;
    
    int GetLineNumber() const;
    
    int GetColumnNumber() const;
    
  protected:
    std::ostream & PrintLineAndColumn(std::ostream & out) const;
    
    int mLineNumber;
    int mColumnNumber;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
