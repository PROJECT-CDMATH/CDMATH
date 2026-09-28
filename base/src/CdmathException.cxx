#include "CdmathException.hxx"

using namespace std;

CdmathException::CdmathException(std::string reason):_reason(reason)
{
}

CdmathException::CdmathException(std::string reason, std::string file, int line):_reason(reason)
{
}

CdmathException::~CdmathException() noexcept
{
}

const char *CdmathException::what() const noexcept
{
  return _reason.c_str();
}
