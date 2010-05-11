/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include "connector_test.hpp"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION(ConnectorTest);

using namespace std;

/* ConnectorTest public methods */
void ConnectorTest::setUp()
{
  CPPUNIT_ASSERT(create_listener(mServer, 0, "127.0.0.1") == 0);
  mPort = mServer->get_listening_port();
  mConnector.reset(new TestConnector("127.0.0.1", mPort));
}

void ConnectorTest::thread()
{
  mConnector->connect();
}

void ConnectorTest::tearDown()
{
  mServer.reset();
  mServerSocket.reset();
  dlib::sleep(500);
  mConnector.reset();
}

/* ConnectorTest protected methods */
void ConnectorTest::testConnection()
{
  // Start the accept thread
  start();
  
  CPPUNIT_ASSERT_EQUAL(0, mServer->accept(mServerSocket, 1000));
  CPPUNIT_ASSERT(mServerSocket.get() != NULL);
}

void ConnectorTest::testDataCapture()
{
  // Start the accept thread
  start();
  
  CPPUNIT_ASSERT_EQUAL(0, mServer->accept(mServerSocket, 1000));
  CPPUNIT_ASSERT(mServerSocket.get() != NULL);

  string command("Hello Connector\n");
  CPPUNIT_ASSERT((size_t) mServerSocket->write(command.c_str(), command.length()) == command.length());
  dlib::sleep(500);

  // \n is stripped from the posted data.
  CPPUNIT_ASSERT_EQUAL(command.substr(0, command.length() - 1), mConnector->mData); 
}

void ConnectorTest::testDisconnect()
{
  // Start the accept thread
  start();
  
  CPPUNIT_ASSERT_EQUAL(0, mServer->accept(mServerSocket, 1000));
  CPPUNIT_ASSERT(mServerSocket.get() != NULL);
  CPPUNIT_ASSERT(!mConnector->mDisconnected);
  mServerSocket.reset();
  dlib::sleep(500);
  CPPUNIT_ASSERT(mConnector->mDisconnected);
}

void ConnectorTest::testProtocolCommand()
{
  // Start the accept thread
  start();
  
  CPPUNIT_ASSERT_EQUAL(0, mServer->accept(mServerSocket, 1000));
  CPPUNIT_ASSERT(mServerSocket.get() != NULL);

  string command("* Hello Connector\n");
  CPPUNIT_ASSERT((size_t) mServerSocket->write(command.c_str(), command.length()) == command.length());
  dlib::sleep(500);

  // \n is stripped from the posted data.
  CPPUNIT_ASSERT_EQUAL(command.substr(0, command.length() - 1), mConnector->mCommand); 
}

void ConnectorTest::testHeartbeat()
{
  // Start the accept thread
  start();
  
  CPPUNIT_ASSERT_EQUAL(0, mServer->accept(mServerSocket, 1000));
  CPPUNIT_ASSERT(mServerSocket.get() != NULL);

  // Receive initial heartbeat request "* PING\n"
  mServerSocket->
}


