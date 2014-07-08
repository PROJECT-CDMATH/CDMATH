#include "CdmathException.hxx"

CdmathException::CdmathException(const char *reason):_reason(reason)
{
}

CdmathException::CdmathException(const char *reason, const char *file, int line):_reason(reason)
{
}

CdmathException::~CdmathException() throw ()
{
}

const char *CdmathException::what() const throw()
{
  return _reason.c_str();
}
