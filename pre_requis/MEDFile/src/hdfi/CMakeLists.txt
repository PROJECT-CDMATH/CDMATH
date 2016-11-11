SET(hdfi_SOURCES 
_MEDfilterEntityFullIGlobalCr.c 
_MEDfilterEntityFullICompactCr.c 
_MEDfilterEntityNoIGlobalCr.c 
_MEDfilterEntityNoICompactCr.c 
_MEDselectAllEntitiesFullI.c 
_MEDselectAllEntitiesNoI.c 
_MEDfilterBlockOfEntityFullIGlobalCr.c 
_MEDfilterBlockOfEntityNoIGlobalCr.c 
_MEDfilterBlockOfEntityFullICompactCr.c 
_MEDfilterBlockOfEntityNoICompactCr.c 
_MEDdatasetWr.c 
_MEDdatasetRd.c 
_MEDfileOpen.c 
_MEDfileName.c 
_MEDmemFileOpen.c 
_MEDfileCreate.c 
_MEDnObjects.c 
__MEDobjectGetName.c 
_MEDobjectCrOrderGetName.c 
_MEDobjectGetName.c 
_MEDcopyName.c 
_MEDvisit.c 
_MEDlinkObjs.c 
_MEDgetComputationStepName.c 
_MEDdatagroupCrOrderCr.c 
_MEDdatagroupOpen.c 
_MEDsoftlinkDel.c 
_MEDisasoftlink.c 
_MEDgetDatasetChgt.c 
_MEDdatasetExistByMedtype.c 
_MEDattributeStringRdByName.c 
_MEDattributeNumRdByName.c 
_MEDattributeStringWrByName.c 
_MEDattributeStringWr.c 
_MEDattributeNumWrByName.c 
_MEDattributeNumWr.c 
_MEDcheckAttributeStringInit.c 
_MEDcheckAttributeStringFin.c 
_MEDcheckAttributeStringFunc.c 
_MEDchecknSublinkFunc.c 
_MEDiterate.c 
_MEDobjectGetRank.c 
_MEDdatagroupExist.c 
_MEDdatasetExist.c 
_MEDattributeExist.c
MEDattrFermer.c 
MEDattrNumLire.c 
MEDattrOuvrir.c 
MEDattrStringLire.c 
MEDdatagroupCreer.c 
MEDdatagroupFermer.c 
MEDdatagroupOuvrir.c 
MEDdatagroupLienSupprimer.c 
MEDdatagroupLienCreer.c 
MEDdatasetFermer.c 
MEDdatasetOuvrir.c 
MEDfichierFermer.c 
MEDfichierNo.c 
MEDmodeErreurVerrouiller.c 
MEDfichierMonter.c 
MEDfichierDemonter.c 
MEDobjetsOuverts.c
  )

IF(MEDFILE_USE_MPI)
  LIST(APPEND hdfi_SOURCES _MEDparFileOpen.c _MEDparFileCreate.c)
ENDIF(MEDFILE_USE_MPI)
  
ADD_LIBRARY(_hdfi OBJECT ${hdfi_SOURCES})
 
ADD_LIBRARY(_hdfi_static OBJECT ${hdfi_SOURCES})
MED_SET_DEFINITIONS(_hdfi_static MED3_USESTATIC)
