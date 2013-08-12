### Makefile ---

## Author: geoff@minty-dark-tower
## Version: $Id: Makefile,v 0.0 2013/08/11 06:22:54 geoff Exp $
## Keywords:
## X-URL:

test: test.cpp named_pipe.hpp impl/named_pipe_unix.hpp
	g++ test.cpp -g -o test

### Makefile ends here
