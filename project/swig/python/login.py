#!/usr/bin/env python

import sys
import librets

try:
  session = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")
  if (not session.Login("Joe", "Schmoe")):
      print "Invalid login"
      sys.exit(1)

  print "Member name: ", session.GetLoginResponse().GetMemberName()
  print "Search URL: ", session.GetCapabilityUrls().GetSearchUrl()
  print "Action: ", session.GetAction()
  version = "1.0"
  if session.GetDetectedRetsVersion() == librets.RETS_1_5:
    version = "1.5"
  print "RETS Version: " + version

  logout = session.Logout();
  print "Billing info: " + logout.GetBillingInfo()
  print "Logout message: " + logout.GetLogoutMessage()
  print "Connect time: " + str(logout.GetConnectTime())
except librets.RetsException,e:
  print "Caught: " + e.GetMessage()