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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "TestSqlMetadata.h"
#include "librets/SqlToDmqlCompiler.h"
#include "librets/GetObjectQuery.h"
#include "librets/DmqlExpression.h"
#include "librets/RetsSqlException.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS SqlToDmqlObjectTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testGetAllObjects);
    CPPUNIT_TEST(testGetOneObject);
    CPPUNIT_TEST(testGetTwoObjects);
    CPPUNIT_TEST(testGetUsingLocation);
    CPPUNIT_TEST(testGetUsingBinary);
    CPPUNIT_TEST(testBadObjectTable);
    CPPUNIT_TEST(testCountWithObject);
    CPPUNIT_TEST(testNoWhere);
    CPPUNIT_TEST(testQuestionMark);
    CPPUNIT_TEST(testObjectKeyIn);
    CPPUNIT_TEST_SUITE_END();

  public:
    CLASS();

  protected:
    void testGetAllObjects();
    void testGetOneObject();
    void testGetTwoObjects();
    void testGetUsingLocation();
    void testGetUsingBinary();
    void testBadObjectTable();
    void testCountWithObject();
    void testNoWhere();
    void testQuestionMark();
    void testObjectKeyIn();
    
    GetObjectQueryPtr sqlToGetObject(string sql);
    void assertInvalidSql(string sql,
                          const CPPUNIT_NS::SourceLine & sourceLine);
    SqlToDmqlCompilerPtr mCompiler;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

CLASS::CLASS()
{
    TestSqlMetadataPtr metadata(new TestSqlMetadata());
    mCompiler.reset(new SqlToDmqlCompiler(metadata));
}

GetObjectQueryPtr CLASS::sqlToGetObject(string sql)
{
    if (mCompiler->sqlToDmql(sql) != SqlToDmqlCompiler::GET_OBJECT_QUERY)
    {
        CPPUNIT_FAIL("Not a GetObject query");
    }
    return mCompiler->GetGetObjectQuery();
}

#define ASSERT_INVALID_SQL(_SQL_) assertInvalidSql(_SQL_, CPPUNIT_SOURCELINE())

void CLASS::assertInvalidSql(string sql,
                             const CPPUNIT_NS::SourceLine & sourceLine)
{
    try
    {
        istringstream inputStream(sql);
        mCompiler->sqlToDmql(inputStream);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message("forced failure",
                                "should have thrown exception"),
            sourceLine);
    }
    catch (RetsSqlException &)
    {
        // Expected
    }
}

void CLASS::testGetAllObjects()
{
    GetObjectQueryPtr query = 
        sqlToGetObject("select * "
                       "  from object:location:Property "
                       " where type = 'Photo' "
                       "       and object_key = 'LN1'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testGetOneObject()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:location:Property "
                   " where type = 'Photo' and object_key = 'LN1:1'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1:1");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testGetTwoObjects()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:location:Property "
                   " where type = 'Photo' and object_key = 'LN1:1' or"
                   "              object_key = 'LN1:2'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1:1");
    objectKeys.push_back("LN1:2");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testGetUsingLocation()
{
    GetObjectQueryPtr query = 
        sqlToGetObject("select * "
                       "  from object:location:Property "
                       " where type = 'Photo' "
                       "       and object_key = 'LN1'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}    

void CLASS::testGetUsingBinary()
{
    GetObjectQueryPtr query = 
        sqlToGetObject("select * "
                       "  from object:binary:Property "
                       " where type = 'Photo' "
                       "       and object_key = 'LN1'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(false, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testBadObjectTable()
{
    ASSERT_INVALID_SQL(
        "select * "
        "  from object:fail:Property "
        " where type = 'Photo' "
        "       and object_key = 'LN1'");
}

void CLASS::testCountWithObject()
{
    ASSERT_INVALID_SQL(
        "select count(*) "
        "  from object:location:Property "
        " where type = 'Photo' "
        "       and object_key = 'LN1'");
}

void CLASS::testNoWhere()
{
    GetObjectQueryPtr query =
        sqlToGetObject("SELECT * "
                       "FROM object:binary:Property");

    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("", query->GetType());
    ASSERT_EQUAL(false, query->GetUseLocation());

    StringVector objectKeys;
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testQuestionMark()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:location:Property "
                   " where type = 'Photo' and object_key = ?");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("?");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}

void CLASS::testObjectKeyIn()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:location:Property "
                   " where type = 'Photo' and object_key IN "
                   "       ('LN1:1', 'LN1:2', 'LN2:1')");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_EQUAL(true, query->GetUseLocation());

    StringVector objectKeys;
    objectKeys.push_back("LN1:1");
    objectKeys.push_back("LN1:2");
    objectKeys.push_back("LN2:1");
    ASSERT_VECTOR_EQUAL(objectKeys, *query->GetObjectKeys());
}
