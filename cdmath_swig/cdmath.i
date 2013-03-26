%module cdmath

%pythoncode %{
def FieldIadd(self,*args):
    import _cdmath
    return _cdmath.Field____iadd___(self,self, *args)
def FieldIsub(self,*args):
    import _cdmath
    return _cdmath.Field____isub___(self,self, *args)
def FieldImul(self,*args):
    import _cdmath
    return _cdmath.Field____imul___(self,self, *args)
def FieldIdiv(self,*args):
    import _cdmath
    return _cdmath.Field____idiv___(self,self, *args)

def IntTabIadd(self,*args):
    import _cdmath
    return _cdmath.IntTab____iadd___(self,self, *args)
def IntTabIsub(self,*args):
    import _cdmath
    return _cdmath.IntTab____isub___(self,self, *args)
def IntTabImul(self,*args):
    import _cdmath
    return _cdmath.IntTab____imul___(self,self, *args)
def IntTabIdiv(self,*args):
    import _cdmath
    return _cdmath.IntTab____idiv___(self,self, *args)

def DoubleTabIadd(self,*args):
    import _cdmath
    return _cdmath.DoubleTab____iadd___(self,self, *args)
def DoubleTabIsub(self,*args):
    import _cdmath
    return _cdmath.DoubleTab____isub___(self,self, *args)
def DoubleTabImul(self,*args):
    import _cdmath
    return _cdmath.DoubleTab____imul___(self,self, *args)
def DoubleTabIdiv(self,*args):
    import _cdmath
    return _cdmath.DoubleTab____idiv___(self,self, *args)
%}

%{
#include "IntTab.hxx"
#include "DoubleTab.hxx"
#include "Point.hxx"
#include "Node.hxx"
#include "Cell.hxx"
#include "Face.hxx"
#include "Mesh.hxx"
#include "Field.hxx"

#include <sstream>

%}

%feature("autodoc", "1");
%feature("docstring");

#define MEDCOUPLING_EXPORT

%include "MEDCouplingCommon.i"
%include std_string.i

%include "IntTab.hxx"
%include "DoubleTab.hxx"
%include "Point.hxx"
%include "Node.hxx"
%include "Cell.hxx"
%include "Face.hxx"
%include "Mesh.hxx"
%include "Field.hxx"

%extend IntTab
{
  PyObject *___iadd___(PyObject *trueSelf, const IntTab& f)
  {
    (*self)+=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___iadd___(PyObject *trueSelf, double value)
  {
    (*self)+=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, const IntTab& f)
  {
    (*self)-=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, double value)
  {
    (*self)-=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___imul___(PyObject *trueSelf, double val)
  {
    (*self)*=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___idiv___(PyObject *trueSelf, double val)
  {
    (*self)/=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  std::string __repr__()
  {
     std::ostringstream oss;
     oss << "Number of Values : " << (*self).size() << std::endl;
     oss << "Data content : " << std::endl;
     for (int i=0;i<(*self).size();i++)
         oss << "# " << i << " : " << (*self)[i] << std::endl;
     return oss.str();
  }
  
  int __getitem__(int i)
  {
    const int& tmp=(*self)[i];
    return int(tmp);
  }

  void __setitem__(int i, int val)
  {
    (*self)[i]=val;
  }

  PyObject *getValues()
  {
     const int *vals=self->getValues();
     return convertIntArrToPyList(vals,self->size());
  }

}

%extend DoubleTab
{
  PyObject *___iadd___(PyObject *trueSelf, const DoubleTab& f)
  {
    (*self)+=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___iadd___(PyObject *trueSelf, double value)
  {
    (*self)+=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, const DoubleTab& f)
  {
    (*self)-=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, double value)
  {
    (*self)-=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___imul___(PyObject *trueSelf, double val)
  {
    (*self)*=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___idiv___(PyObject *trueSelf, double val)
  {
    (*self)/=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  std::string __repr__()
  {
     std::ostringstream oss;
     oss << "Number of Values : " << (*self).size() << std::endl;
     oss << "Data content : " << std::endl;
     for (int i=0;i<(*self).size();i++)
         oss << "# " << i << " : " << (*self)[i] << std::endl;
     return oss.str();
  }

  double __getitem__(int i)
  {
    const double& tmp=(*self)[i];
    return double(tmp);
  }

  void __setitem__(int i, double val)
  {
    (*self)[i]=val;
  }

  PyObject *getValues()
  {
     const double *vals=self->getValues();
     return convertDblArrToPyList(vals,self->size());
  }

}

%extend Point
{
//  std::string __repr__()
//    {
//     std::ostringstream oss; oss << *self;
//      return oss.str();
//    }

  double __getitem__(int i)
  {
    const double& tmp=(*self)[i];
    return double(tmp);
  }

  void __setitem__(int i, double val)
  {
    (*self)[i]=val;
  }
}

%extend Mesh
{
  std::string __repr__()
  {
     std::ostringstream oss;
     oss << self->getMeshU()->simpleRepr();
     return oss.str();
    }

}

%extend Field
{
  PyObject *___iadd___(PyObject *trueSelf, const Field& f)
  {
    (*self)+=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___iadd___(PyObject *trueSelf, double value)
  {
    (*self)+=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, const Field& f)
  {
    (*self)-=f;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___isub___(PyObject *trueSelf, double value)
  {
    (*self)-=value;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___imul___(PyObject *trueSelf, double val)
  {
    (*self)*=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *___idiv___(PyObject *trueSelf, double val)
  {
    (*self)/=val;
    Py_XINCREF(trueSelf);
    return trueSelf;
  }

  PyObject *getValues()
  {
     const double *vals=self->getValues();
     return convertDblArrToPyList(vals,self->getNumberOfElements()*self->getNumberOfComponents());
  }

  double __getitem__(PyObject *value)
  {
    int sz=PyTuple_Size(value);
    PyObject *ob1=PyTuple_GetItem(value,0);
    PyObject *ob2=PyTuple_GetItem(value,1);
    int i=(int)PyInt_AS_LONG(ob1);
    int j=(int)PyInt_AS_LONG(ob2);
    const double& tmp=(*self)(i,j);
    return double(tmp);
  }

  double __getitem__(int i)
  {
    const double& tmp=(*self)[i];
    return double(tmp);
  }

  void __setitem__(int i, double val)
  {
    (*self)[i]=val;
  }

  void __setitem__(PyObject *ob, double value)
  {
    int sz=PyTuple_Size(ob);
    PyObject *ob1=PyTuple_GetItem(ob,0);
    PyObject *ob2=PyTuple_GetItem(ob,1);
    int i=(int)PyInt_AS_LONG(ob1);
    int j=(int)PyInt_AS_LONG(ob2);
    (*self)(i,j)=value;
  }

  std::string __repr__()
  {
     std::ostringstream oss;
     oss << self->getField()->simpleRepr();
     return oss.str();
    }
}
%pythoncode %{
Field.__iadd__=FieldIadd
Field.__isub__=FieldIsub
Field.__imul__=FieldImul
Field.__idiv__=FieldIdiv
IntTab.__iadd__=IntTabIadd
IntTab.__isub__=IntTabIsub
IntTab.__imul__=IntTabImul
IntTab.__idiv__=IntTabIdiv
DoubleTab.__iadd__=DoubleTabIadd
DoubleTab.__isub__=DoubleTabIsub
DoubleTab.__imul__=DoubleTabImul
DoubleTab.__idiv__=DoubleTabIdiv
%}

