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

#ifndef LIBRETS_SQL_METADATA_H
#define LIBRETS_SQL_METADATA_H
/** 
 * @file SqlMetadata.h
 * (Internal) Contains the SqlMetadata class definition.
 */
 /// @cond MAINTAINER
#include <string>

namespace librets {
    
class SqlMetadata
{
  public:
    virtual ~SqlMetadata() { };
    
    virtual bool IsLookupColumn(std::string tableName, std::string columnName)
        = 0;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
