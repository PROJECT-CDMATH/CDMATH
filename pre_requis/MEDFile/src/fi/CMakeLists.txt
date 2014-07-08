SET(fi_SOURCES
  medsupport.f
  medequivalence.f
  medlink.f
  medinterp.f
  medstructelement.f
  medfilter.f
  medfile.f
  medlocalization.f
  medprofile.f
  medjoint.f
  medparameter.f
  medfamily.f
  medfield.f
  medflibrary.f
  medmesh.f
)

ADD_LIBRARY(_fi        OBJECT ${fi_SOURCES})
