//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: main.cpp,v $
// $Revision: 1.3 $
// $Date: 2012/10/14 01:21:29 $
//
// Permission to use, copy, modify, distribute, and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appear in all copies and that both the
// copyright notice and this permission notice appear in supporting
// documentation.  No representations are made about the suitability of this
// software for any purpose.  It is provided "as is" without express or 
// implied warranty.
//

#include "fslurp.h"

int
main(int argc, char *argv[])
{
	Args		args(argc, argv);
	Comm		comm(args);
	Interface	interface(&comm);

	interface.initialize();
	interface.discoverDevices();

	Report	report(&args, &interface);
	report.report();

	return 0;
}
