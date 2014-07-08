# -*- coding:utf-8 -*-
%module medfile

//A inclure avant toute déclaration à SWIG
%include "med_881.i"
%SwigPyIteratorDef(medfile)

%include "med_common.i"

%{
#include "medfile.h"
%}

%apply int *OUTPUT { med_int *const major, med_int* const minor, med_int* const release };
//Pour éviter le mapping sur le freearg de la macro med_array_typemaps (med_common.i)
%typemap(freearg) (med_int * const ) {}

%cstring_bounded_output(char* const version,MED_NAME_SIZE);
%cstring_bounded_output(char* const comment,MED_COMMENT_SIZE);
%cstring_bounded_mutable(const char * const comment,MED_COMMENT_SIZE);

//TODO : MED_MAX_CHFID_PATH
//%cstring_bounded_mutable(const char * const chpath  ,MED_PATHNAME_SIZE);

%include "medfile_exception.i"

%include "medfile.h"

// Fin Du Mapping.

// Liste des arguments apparaissant dans medfile_proto.h
//char* const comment
//char* const version
//const char * const chpath
//const char* const comment
//const char* const filename
//const med_access_mode accessmode
//const med_class medclass
//const med_idt chfid
//const med_idt fid
//const med_idt mid
//med_bool* const hdfok
//med_bool* const medok
//med_idt fid
//med_int* const major
//med_int* const minor
//med_int* const release
//
