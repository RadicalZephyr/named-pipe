Named Pipe for Boost.Interprocess
=================================

This is the repo for an ISP I'm doing on building a basic
cross-platform named-pipe implementation.  One of the design goals is
that it eventually be included in the Boost.Interprocess library.

Ideas and suggestions (and pull requests!) welcome!

Thanks for their help in refining the interface to: Rob Stewart and
Edward Diener.


Update 8/16/2013
----------------

Ok, so I've been convinced by Niall Douglas, and John Vernarchick from
the mailing list that an ASIO implementation is actually the
appropriate way to go.  However, since I only have a couple more days
to work on my independent study, I'm going to continue with the raw
implementation for now.  I'm going to treat this as research though,
and a generally good learning experience, but the actual code for a
cross-platform named pipe will eventually be based on ASIO code and be
compatible with inclusion in the Boost.ASIO library.
