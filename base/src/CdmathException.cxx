#include "CdmathException.hxx"

using namespace std;

CdmathException::CdmathException(string reason):_reason(reason)
{
}

CdmathException::CdmathException(string reason,string file, int line):_reason(reason)
{
}

CdmathException::~CdmathException() throw ()
{
}

const char *CdmathException::what() const throw()
{
  return _reason.c_str();
}
