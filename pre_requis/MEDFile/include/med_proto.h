/*  This file is part of MED.
 *
 *  COPYRIGHT (C) 1999 - 2012  EDF R&D, CEA/DEN
 *  MED is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MED is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MED.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MED_PROTO_H
#define MED_PROTO_H

#include "medC_win_dll.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Interface de l'API MED */
/* Library */
MEDC_EXPORT med_err
MEDlibraryNumVersion(med_int* const major,
		     med_int* const minor,
		     med_int* const release);

MEDC_EXPORT med_err
MEDlibraryStrVersion(char* const medversion);

MEDC_EXPORT med_err
MEDlibraryHdfNumVersion(med_int* const major,
			med_int* const minor,
			med_int* const release);
MEDC_EXPORT med_err
MEDlibraryHdfStrVersion(char* const  version);
MEDC_EXPORT med_err
MEDlibraryClose(void);

/* File */
MEDC_EXPORT med_idt
MEDfileOpen(const char* const filename,
	    const med_access_mode accessmode);

#ifdef MED_HAVE_MPI
MEDC_EXPORT med_idt
MEDparFileOpen(const char* const filename,
	       const med_access_mode accessmode,
	       const MPI_Comm comm, const MPI_Info info);

#endif

MEDC_EXPORT med_err
MEDfileClose(med_idt fid);

MEDC_EXPORT med_err
MEDfileCommentWr(const med_idt fid,
		 const char* const comment);
MEDC_EXPORT med_err
MEDfileCommentRd(const med_idt fid,
		 char* const comment);
MEDC_EXPORT med_err
MEDfileCompatibility(const char* const filename,
		     med_bool* const hdfok,
		     med_bool* const medok);
MEDC_EXPORT med_err
MEDfileNumVersionRd(const med_idt fid,
		    med_int* const major,
		    med_int* const minor,
		    med_int* const release);
MEDC_EXPORT med_err
MEDfileStrVersionRd(const med_idt fid,
		    char* const version);
MEDC_EXPORT med_idt
MEDfileObjectsMount(const med_idt fid,
		    const char* const filename,
		    const med_class medclass);
MEDC_EXPORT med_err
MEDfileObjectsUnmount(const med_idt fid,
		      const med_idt mid,
		      const med_class medclass);

/* Mesh */

/*routine interne*/
MEDC_EXPORT med_err
_MEDmeshCr(const med_idt fid, const char * const root,
	   const char * const meshname, const med_int spacedim,
	   const med_int meshdim, const med_mesh_type meshtype,
	   const char * const description, const char * const dtunit,
	   const med_sorting_type sortingtype,
	   const med_axis_type axistype, const char * const axisname,
	   const char * const axisunit);

MEDC_EXPORT med_err
MEDmeshCr(const med_idt fid,
	  const char * const meshname, const med_int spacedim,
	  const med_int meshdim, const med_mesh_type meshtype,
	  const char * const description, const char * const dtunit,
	  const med_sorting_type sortingtype,
	  const med_axis_type axistype, const char * const axisname,
	  const char * const axisunit);

/*routine interne*/
MEDC_EXPORT med_err
_MEDmeshInfoByName(const med_idt fid, const char * const root,const char * const meshname,
		   med_int * const spacedim,med_int * const meshdim,  med_mesh_type * const meshtype,
		   char * const description,  char * const dtunit,
		   med_sorting_type *const sortingtype,
		   med_int * const nstep,  med_axis_type * const axistype,  char * const axisname,
		   char * const axisunit);

/*routine interne*/
MEDC_EXPORT med_idt
_MEDmeshAssociatedGroupCr(const med_idt               id,
                          const char*  const          rootname,
                          const char*  const          meshname,
                          const med_int               numdt,
                          const med_int               numit,
                          const med_float             dt,
 			  const med_bool              justopen,
                          const char*  const          datagroupname );

MEDC_EXPORT med_err
MEDmeshInfoByName(const med_idt fid,const char * const meshname,
		  med_int * const spacedim, med_int * const meshdim,  med_mesh_type * const meshtype,
		  char * const description,  char * const dtunit,
		  med_sorting_type * const sortingtype,
		  med_int * const nstep,  med_axis_type * const axistype,  char * const axisname,
		  char * const axisunit);

MEDC_EXPORT med_err
MEDmeshInfo(const med_idt            fid,
	    const int                meshit,
	    char   *           const meshname,
	    med_int *          const spacedim,
	    med_int *          const meshdim,
	    med_mesh_type *    const meshtype,
	    char *             const description,
	    char *             const dtunit,
	    med_sorting_type * const sortingtype,
	    med_int *          const nstep,
	    med_axis_type *    const axistype,
	    char *             const axisname,
	    char *             const axisunit);

MEDC_EXPORT med_int
MEDnMesh(const med_idt fid);

MEDC_EXPORT med_int
MEDmeshnAxis(const med_idt fid, const int meshit);

MEDC_EXPORT med_int
MEDmeshnAxisByName(const med_idt fid, const char * const meshname);

MEDC_EXPORT med_err
MEDmeshGridTypeWr(const med_idt fid,const char * const meshname, const med_grid_type gridtype);

MEDC_EXPORT med_err
MEDmeshGridTypeRd(const med_idt fid,const char * const meshname, med_grid_type * const gridtype);

MEDC_EXPORT med_err
MEDmeshGridIndexCoordinateWr(const med_idt               fid,
			     const char*  const          meshname,
			     const med_int               numdt,
			     const med_int               numit,
			     const med_float             dt,
			     const med_int               axis,
			     const med_int               indexsize,
			     const med_float * const     gridindex);
MEDC_EXPORT med_err
MEDmeshGridIndexCoordinateRd(const med_idt               fid,
			     const char*  const          meshname,
			     const med_int               numdt,
			     const med_int               numit,
			     const med_int               axis,
			     med_float * const     gridindex);

MEDC_EXPORT med_err
MEDmeshGridStructWr(const med_idt               fid,
		    const char*  const          meshname,
		    const med_int               numdt,
		    const med_int               numit,
		    const med_float             dt,
		    const med_int * const       gridstruct);

MEDC_EXPORT med_err
MEDmeshGridStructRd(const med_idt               fid,
		    const char*  const          meshname,
		    const med_int               numdt,
		    const med_int               numit,
		    med_int * const       gridstruct);

MEDC_EXPORT med_err
MEDmeshUniversalNameWr(const med_idt fid, const char * const meshname);

MEDC_EXPORT med_err
MEDmeshUniversalNameRd(const med_idt fid, const char * const meshname,char * const univname);

MEDC_EXPORT med_err
MEDmeshComputationStepCr(const med_idt fid,const char * const meshname,
			 const med_int numdt1, const med_int numit1,
			 const med_int numdt2, const med_int numit2,
			 const med_float dt2 );

MEDC_EXPORT med_err
MEDmeshAttributeRd(const med_idt fid, const char * const meshname,
		   med_int *isolatednodes, med_int *verticesnodes, med_int *cellmaxnodes);
MEDC_EXPORT med_err
MEDmeshAttributeWr(const med_idt fid, const char * const meshname,
		   const med_int isolatednodes,const med_int verticesnodes,const med_int cellmaxnodes);

MEDC_EXPORT med_err
MEDmeshComputationStepDtRd(const med_idt fid,const char * const meshname,
			   const med_int numdt, const med_int umit, med_float * dt );

MEDC_EXPORT med_err
MEDmeshComputationStepInfo(const med_idt fid,
			   const char * const meshname,
			   const int csit,
			   med_int *numdt,
			   med_int *numit,
			   med_float * dt );

MEDC_EXPORT med_err
MEDmeshSortingTypeRd(const med_idt               fid,
		     const char*  const          meshname,
		     med_sorting_type * const    sortingtype );

/*interne*/
MEDC_EXPORT med_err
_MEDmeshAdvancedWr(const med_idt               fid,
		   const char*  const          meshname,
		   const med_data_type         meddatatype,
		   const char*  const          datasetname,
		   const med_internal_type     datatype,
		   const med_int               numdt,
		   const med_int               numit,
		   const med_float             dt,
		   const med_entity_type       entitype,
		   const med_geometry_type     geotype,
		   const med_connectivity_mode cmode,
		   const med_storage_mode      storagemode,
		   const char * const          profilename,
		   const med_switch_mode       switchmode,
		   const med_int               dimselect,
		   const med_filter * const    filter,
		   const med_int               nentity,
		   const void * const          value);
/*fin interne */

MEDC_EXPORT med_err
MEDmeshNodeCoordinateAdvancedWr(const med_idt               fid,
				const char*  const          meshname,
				const med_int               numdt,
				const med_int               numit,
				const med_float             dt,
				const med_filter * const    filter,
				const med_float* const  value);
MEDC_EXPORT med_err
MEDmeshNodeCoordinateWithProfileWr(const med_idt               fid,
				   const char*  const          meshname,
				   const med_int               numdt,
				   const med_int               numit,
				   const med_float             dt,
				   const med_storage_mode      storagemode,
				   const char * const          profilename,
				   const med_switch_mode       switchmode,
				   const med_int               dimselect,
				   const med_int               nentity,
				   const med_float* const      coordinates);
MEDC_EXPORT med_err
MEDmeshNodeCoordinateWr(const med_idt               fid,
				   const char*  const          meshname,
				   const med_int               numdt,
				   const med_int               numit,
				   const med_float             dt,
				   const med_switch_mode       switchmode,
				   const med_int               nentity,
				   const med_float* const  coordinates);

MEDC_EXPORT med_err
MEDmeshNodeCoordinateTrsfWr(const med_idt               fid,
			    const char*  const          meshname,
			    const med_int               numdt,
			    const med_int               numit,
			    const med_float             dt,
			    const med_float* const      coordinatetrsf);
MEDC_EXPORT med_err
MEDmeshNodeCoordinateTrsfRd(const med_idt               fid,
				    const char*  const          meshname,
				    const med_int               numdt,
				    const med_int               numit,
				    const med_float* const      coordinatetrsf);
MEDC_EXPORT med_err
MEDmeshElementConnectivityWr(const med_idt               fid,
			     const char*  const          meshname,
			     const med_int               numdt,
			     const med_int               numit,
			     const med_float             dt,
			     const med_entity_type       entitype,
			     const med_geometry_type     geotype,
			     const med_connectivity_mode cmode,
			     const med_switch_mode       switchmode,
			     const med_int               nentity,
			     const med_int* const  connectivity);

MEDC_EXPORT med_err
MEDmeshElementConnectivityAdvancedWr(const med_idt               fid,
				     const char*  const          meshname,
				     const med_int               numdt,
				     const med_int               numit,
				     const med_float             dt,
				     const med_entity_type       entitype,
				     const med_geometry_type     geotype,
				     const med_connectivity_mode cmode,
				     const med_filter * const    filter,
				     const med_int* const  connectivity);
MEDC_EXPORT med_err
MEDmeshElementConnectivityWithProfileWr(const med_idt               fid,
					const char*  const          meshname,
					const med_int               numdt,
					const med_int               numit,
					const med_float             dt,
					const med_entity_type       entitype,
					const med_geometry_type     geotype,
					const med_connectivity_mode cmode,
					const med_storage_mode      storagemode,
					const char * const          profilename,
					const med_switch_mode       switchmode,
					const med_int               dimselect,
					const med_int               nentity,
					const med_int* const  connectivity);
/*routine interne*/
MEDC_EXPORT med_err
_MEDmeshAdvancedRd(const med_idt               fid,
		   const char*  const          meshname,
		   const med_data_type         meddatatype,
		   const char*  const          datasetname,
		   const med_internal_type     datatype,
		   const med_int               numdt,
		   const med_int               numit,
		   const med_entity_type       entitype,
		   const med_geometry_type     geotype,
		   const med_connectivity_mode cmode,
		   const med_storage_mode      storagemode,
		   const char * const          profilename,
		   const med_switch_mode       switchmode,
		   const med_int               dimselect,
		   const med_filter * const    filter,
		   unsigned char* const  value);

MEDC_EXPORT med_err
MEDmeshNodeCoordinateAdvancedRd(const med_idt               fid,
				const char*  const          meshname,
				const med_int               numdt,
				const med_int               numit,
				const med_filter * const    filter,
				med_float* const  value);
MEDC_EXPORT med_err
MEDmeshNodeCoordinateWithProfileRd(const med_idt               fid,
				   const char*  const          meshname,
				   const med_int               numdt,
				   const med_int               numit,
				   const med_storage_mode      storagemode,
				   const char * const          profilename,
				   const med_switch_mode       switchmode,
				   const med_int               dimselect,
				   med_float* const  coordinates);
MEDC_EXPORT med_err
MEDmeshNodeCoordinateRd(const med_idt               fid,
			const char*  const          meshname,
			const med_int               numdt,
			const med_int               numit,
			const med_switch_mode       switchmode,
			med_float* const  coordinates);

MEDC_EXPORT med_err
MEDmeshElementConnectivityRd(const med_idt               fid,
			     const char*  const          meshname,
			     const med_int               numdt,
			     const med_int               numit,
			     const med_entity_type       entitype,
			     const med_geometry_type     geotype,
			     const med_connectivity_mode cmode,
			     const med_switch_mode       switchmode,
			     med_int* const  connectivity);
MEDC_EXPORT med_err
MEDmeshElementConnectivityAdvancedRd(const med_idt               fid,
				     const char*  const          meshname,
				     const med_int               numdt,
				     const med_int               numit,
				     const med_entity_type       entitype,
				     const med_geometry_type     geotype,
				     const med_connectivity_mode cmode,
				     const med_filter * const    filter,
				     med_int* const  connectivity);
MEDC_EXPORT med_err
MEDmeshElementConnectivityWithProfileRd(const med_idt               fid,
					const char*  const          meshname,
					const med_int               numdt,
					const med_int               numit,
					const med_entity_type       entitype,
					const med_geometry_type     geotype,
					const med_connectivity_mode cmode,
					const med_storage_mode      storagemode,
					const char * const          profilename,
					const med_switch_mode       switchmode,
					const med_int               dimselect,
					const med_int               nentity,
					med_int* const  connectivity);
/*Routine interne*/
MEDC_EXPORT med_int
_MEDmeshnEntity(const med_idt fid,
		const char * const meshname,
		const med_int numdt,
		const med_int numit,
		const med_entity_type entitype,
		const med_geometry_type geotype,
		const med_data_type datatype,
		const med_connectivity_mode cmode,
		const med_storage_mode storagemode,
		char     * const profilename,
		med_int  * const profilesize,
		med_bool * const changement,
		med_bool * const transformation );

/*Cr~er une version sans aucun param~tre concernant les s~quences de calcul*/
MEDC_EXPORT med_int
MEDmeshnEntity(const med_idt fid,
	       const char * const meshname,
	       const med_int numdt,
	       const med_int numit,
	       const med_entity_type entitype,
	       const med_geometry_type geotype,
	       const med_data_type datatype,
	       const med_connectivity_mode cmode,
	       med_bool * const changement,
	       med_bool * const transformation );

MEDC_EXPORT med_int
MEDmeshnEntityWithProfile(const med_idt fid,
			  const char * const meshname,
			  const med_int numdt,
			  const med_int numit,
			  const med_entity_type entitype,
			  const med_geometry_type geotype,
			  const med_data_type datatype,
			  const med_connectivity_mode cmode,
			  const med_storage_mode storagemode,
			  char * const profilename,
			  med_int * const profilesize,
			  med_bool * const changement,
			  med_bool * const transformation );

MEDC_EXPORT med_err
MEDmeshEntityInfo(const med_idt                   fid,
		  const char *              const meshname,
		  const med_int                   numdt,
		  const med_int                   numit,
		  const med_entity_type           entitype,
		  const int                       geotypeit,
		  char              *       const geotypename,
		        med_geometry_type * const geotype
		  );

MEDC_EXPORT med_err
MEDmeshEntityNameWr(const med_idt               fid,
		    const char*  const          meshname,
		    const med_int               numdt,
		    const med_int               numit,
		    const med_entity_type       entitype,
		    const med_geometry_type     geotype,
		    const med_int               nentity,
		    const char* const           value);

MEDC_EXPORT med_err
MEDmeshEntityNameRd(const med_idt              fid,
		    const char*  const         meshname,
		    const med_int              numdt,
		    const med_int              numit,
		    const med_entity_type      entitype,
		    const med_geometry_type    geotype,
		    char* const                name);

MEDC_EXPORT med_err
MEDmeshEntityNumberWr(const med_idt               fid,
		      const char*  const          meshname,
		      const med_int               numdt,
		      const med_int               numit,
		      const med_entity_type       entitype,
		      const med_geometry_type     geotype,
		      const med_int               nentity,
		      const med_int * const       value);

MEDC_EXPORT med_err
MEDmeshEntityNumberRd(const med_idt               fid,
		      const char*  const          meshname,
		      const med_int               numdt,
		      const med_int               numit,
		      const med_entity_type       entitype,
		      const med_geometry_type     geotype,
		      med_int * const             number);

MEDC_EXPORT med_err
MEDmeshEntityFamilyNumberWr(const med_idt               fid,
			    const char*  const          meshname,
			    const med_int               numdt,
			    const med_int               numit,
			    const med_entity_type       entitype,
			    const med_geometry_type     geotype,
			    const med_int               nentity,
			    const med_int * const       value);
MEDC_EXPORT med_err
MEDmeshEntityFamilyNumberRd(const med_idt               fid,
			    const char*  const          meshname,
			    const med_int               numdt,
			    const med_int               numit,
			    const med_entity_type       entitype,
			    const med_geometry_type     geotype,
			    med_int * const             number);

MEDC_EXPORT med_err
MEDmeshEntityAttributeAdvancedRd(const med_idt               fid,
				 const char*  const          meshname,
				 const med_data_type         datatype,
				 const med_int               numdt,
				 const med_int               numit,
				 const med_entity_type       entitype,
				 const med_geometry_type     geotype,
				 const med_filter * const    filter,
				 void * const                attval);

MEDC_EXPORT med_err
MEDmeshEntityAttributeAdvancedWr(const med_idt               fid,
				 const char*  const          meshname,
				 const med_data_type         datatype,
				 const med_int               numdt,
				 const med_int               numit,
				 const med_entity_type       entitype,
				 const med_geometry_type     geotype,
				 const med_filter * const    filter,
				 const void * const          attval);

MEDC_EXPORT med_err
MEDmeshPolygonWr(const med_idt               fid,
		 const char*  const          meshname,
		 const med_int               numdt,
		 const med_int               numit,
		 const med_float             dt,
		 const med_entity_type       entitype,
		 const med_connectivity_mode cmode,
		 const med_int               indexsize,
		 const med_int * const       polyindex,
		 const med_int * const       connectivity );

MEDC_EXPORT med_err
MEDmeshPolygonRd(const med_idt               fid,
		 const char*  const          meshname,
		 const med_int               numdt,
		 const med_int               numit,
		 const med_entity_type       entitype,
		 const med_connectivity_mode cmode,
		 med_int * const             polyindex,
		 med_int * const             connectivity );

MEDC_EXPORT med_err
MEDmeshPolyhedronRd(const med_idt               fid,
		    const char*  const          meshname,
		    const med_int               numdt,
		    const med_int               numit,
		    const med_entity_type       entitype,
		    const med_connectivity_mode cmode,
		    med_int * const             faceindex,
		    med_int * const             nodeindex,
		    med_int * const             connectivity );

MEDC_EXPORT med_err
MEDmeshPolyhedronWr(const med_idt               fid,
		    const char*  const          meshname,
		    const med_int               numdt,
		    const med_int               numit,
		    const med_float             dt,
		    const med_entity_type       entitype,
		    const med_connectivity_mode cmode,
		    const med_int               faceindexsize,
		    const med_int * const       faceindex,
		    const med_int               nodeindexsize,
		    const med_int * const       nodeindex,
		    const med_int * const       connectivity );

MEDC_EXPORT med_err
MEDmeshGeotypeName(const med_idt                 fid,
		   const med_geometry_type       geotype,
		   char *                  const geotypename);
MEDC_EXPORT med_err
MEDmeshGeotypeParameter(const med_idt                 fid,
			const med_geometry_type       geotype,
			med_int *               const geodim,
			med_int *               const nnodes);

/*FIELD*/
MEDC_EXPORT med_err
MEDfieldCr( const med_idt fid,
	    const char * const fieldname,
	    const med_field_type fieldtype,
	    const med_int ncomponent,
	    const char * const componentname,
	    const char * const componentunit,
	    const char * const dtunit,
	    const char * const meshname);

MEDC_EXPORT med_err
MEDfieldValueAdvancedWr(const med_idt              fid,
			const char* const          fieldname,
			const med_int              numdt,
			const med_int              numit,
			const med_float            dt,
			const med_entity_type      entitype,
			const med_geometry_type    geotype,
			const char * const         localizationname,
			const med_filter* const    filter,
			const unsigned char* const value);

MEDC_EXPORT med_err
MEDfieldValueWithProfileWr(const med_idt              fid,
			   const char*  const         fieldname,
			   const med_int              numdt,
			   const med_int              numit,
			   const med_float            dt,
			   const med_entity_type      entitype,
			   const med_geometry_type    geotype,
			   const med_storage_mode     storagemode,
			   const char * const         profilename,
			   const char * const         localizationname,
			   const med_switch_mode      switchmode,
			   const med_int              componentselect,
			   const med_int              nentity,
			   const unsigned char* const value);

MEDC_EXPORT med_err
MEDfieldValueWr(const med_idt              fid,
		const char*  const         fieldname,
		const med_int              numdt,
		const med_int              numit,
		const med_float            dt,
		const med_entity_type      entitype,
		const med_geometry_type    geotype,
		const med_switch_mode      switchmode,
		const med_int              componentselect,
		const med_int              nentity,
		const unsigned char* const value);

MEDC_EXPORT med_err
MEDfieldValueAdvancedRd(const med_idt              fid,
			const char*  const         fieldname,
			const med_int              numdt,
			const med_int              numit,
			const med_entity_type      entitype,
			const med_geometry_type    geotype,
			const med_filter* const    filter,
			unsigned char* const value);
MEDC_EXPORT med_err
MEDfieldValueWithProfileRd(const med_idt              fid,
			   const char*  const         fieldname,
			   const med_int              numdt,
			   const med_int              numit,
			   const med_entity_type      entitype,
			   const med_geometry_type    geotype,
			   const med_storage_mode     storagemode,
			   const char * const         profilename,
			   const med_switch_mode      switchmode,
			   const med_int              componentselect,
			   unsigned char* const value);

MEDC_EXPORT med_err
MEDfield23ValueWithProfileRd(const med_idt              fid,
			     const char*  const         fieldname,
			     const med_int              numdt,
			     const med_int              numit,
			     const med_entity_type      entitype,
			     const med_geometry_type    geotype,
			     const char*  const         meshname,
			     const med_storage_mode     storagemode,
			     const char * const         profilename,
			     const med_switch_mode      switchmode,
			     const med_int              componentselect,
			     unsigned char* const value);
MEDC_EXPORT med_err
MEDfieldValueRd(const med_idt              fid,
		const char*  const         fieldname,
		const med_int              numdt,
		const med_int              numit,
		const med_entity_type      entitype,
		const med_geometry_type    geotype,
		const med_switch_mode      switchmode,
		const med_int              componentselect,
		unsigned char* const value);

MEDC_EXPORT med_int
MEDfieldnComponentByName(const med_idt fid, const char * const fieldname);

MEDC_EXPORT med_int
MEDfieldnComponent(const med_idt fid, const int ind);

MEDC_EXPORT med_int
MEDnField(const med_idt fid);

MEDC_EXPORT med_err
MEDfieldInfo(const med_idt fid,const int ind, char * const fieldname,
	     char * const meshname, med_bool * const localmesh,
	     med_field_type * const fieldtype, char * const componentname,
	     char * const componentunit, char * const dtunit, med_int * const ncstp);

MEDC_EXPORT med_err
MEDfieldComputingStepInfo(const med_idt fid,
			  const char * const fieldname,
			  const int csit,
			  med_int * const numdt,
			  med_int * const numit,
			  med_float * const dt);

MEDC_EXPORT med_err
MEDfieldComputingStepMeshInfo(const med_idt fid,
			      const char * const fieldname,
			      const int csit,
			      med_int * const numdt,
			      med_int * const numit,
			      med_float * const dt,
			      med_int * const meshnumdt,
			      med_int * const meshnumit);

MEDC_EXPORT med_err
MEDfield23ComputingStepMeshInfo(const med_idt         fid,
				const char *    const fieldname,
				const int             csit,
				med_int *       const numdt,
				med_int *       const numit,
				med_float *     const dt,
				med_int *       const nmesh,
				char    *       const meshname,
				med_bool*       const localmesh,
				med_int *       const meshnumdt,
				med_int *       const meshnumit);

MEDC_EXPORT med_err
MEDfieldComputingStepMeshWr(const med_idt fid,
			    const char * const fieldname,
			    med_int numdt,
			    med_int numit,
			    med_int meshnumdt,
			    med_int meshnumit);

MEDC_EXPORT med_err
_MEDfieldComputingStepCheck236(med_idt fid,
			       const char * const fieldname,
			       med_size * const ncpst,
			       med_bool checkmultiplemesh, med_bool * const multiplemesh,
			       med_bool checkmeshname, med_bool * const samedefaultmeshname);

MEDC_EXPORT med_err
MEDfieldInfoByName(const med_idt fid,const char * const fieldname,
		   char * const meshname, med_bool * const localmesh,
		   med_field_type * const fieldtype, char * const componentname,
		   char * const componentunit, char * const dtunit, med_int * const ncstp);
/*routine interne*/
MEDC_EXPORT med_int
_MEDfieldnValue(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
		const med_entity_type entitype, const med_geometry_type geotype,
		char * const profilename, const  int profileit,
		const med_storage_mode storagemode,med_int * const profilesize,
		char * const localizationname, med_int * const nintegrationpoint);

MEDC_EXPORT med_int
_MEDfield23nValue(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
		const med_entity_type entitype, const med_geometry_type geotype, const char * const meshname,
		char * const profilename, const  int profileit,
		const med_storage_mode storagemode,med_int * const profilesize,
		char * const localizationname, med_int * const nintegrationpoint);


MEDC_EXPORT med_int
MEDfieldnValueWithProfileByName(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
				const med_entity_type entitype, const med_geometry_type geotype, const char * const profilename,
				const med_storage_mode storagemode,med_int * const profilesize,
				char * const localizationname, med_int * const nintegrationpoint);

MEDC_EXPORT med_int
MEDfieldnValueWithProfile(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
			  const med_entity_type entitype, const med_geometry_type geotype,
			  const int profileit,
			  const med_storage_mode storagemode,char * const profilename ,med_int * const profilesize,
			  char * const localizationname, med_int * const nintegrationpoint);

MEDC_EXPORT med_int
MEDfieldnValue(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
	       const med_entity_type entitype, const med_geometry_type geotype);

MEDC_EXPORT med_int
MEDfield23nValueWithProfile(const med_idt fid, const char * const fieldname,const med_int numdt,const med_int numit,
			    const med_entity_type entitype, const med_geometry_type geotype,const char * const meshname,
			    const int profileit,
			    const med_storage_mode storagemode,char * const profilename ,med_int * const profilesize,
			    char * const localizationname, med_int * const nintegrationpoint);

MEDC_EXPORT med_int
MEDfieldnProfile(const med_idt fid,const char * const fieldname,
		 const med_int numdt, const med_int numit,
		 const med_entity_type enttype, const med_geometry_type geotype,
		 char * const defaultprofilename, char * const defaultlocalizationname);

MEDC_EXPORT med_int
MEDfield23nProfile(const med_idt fid,
		   const char * const fieldname,
		   const med_int numdt,
		   const med_int numit,
		   const med_entity_type   entitype,
		   const med_geometry_type geotype,
		   const int    meshit,
		   char * const meshname,
		   char * const defaultprofilename,
		   char * const defaultlocalizationname);


MEDC_EXPORT med_err
MEDfieldInterpWr(const med_int fid,
		 const char* const fieldname,
		 const char* interpname);

MEDC_EXPORT med_int
MEDfieldnInterp(
		const med_int fid,
		const char* const fieldname);


MEDC_EXPORT med_err
MEDfieldInterpInfo(
		   const med_int       fid,
		   const char*   const fieldname,
		         int           interpit,
		         char*   const interpname
		   );


/* Family  */
MEDC_EXPORT med_err
MEDfamilyCr(const med_idt      fid,
	    const char * const meshname,
	    const char * const familyname,
	    const med_int      familynumber,
	    const med_int      ngroup,
	    const char * const groupname);

MEDC_EXPORT med_int
MEDnFamily(const med_idt      fid,
	   const char * const meshname);

MEDC_EXPORT med_int
MEDnFamilyGroup(const med_idt      fid,
		const char * const meshname,
		const int          famit);

MEDC_EXPORT med_err
MEDfamilyInfo(const med_idt  fid,
	      const char *   meshname,
	      const int      famit,
	      char * const   familyname,
	      med_int *const familynumber,
	      char *const    groupname);

MEDC_EXPORT med_int
MEDnFamily23Attribute(const med_idt      fid,
		      const char * const meshname,
		      const int          famit);

MEDC_EXPORT med_err
MEDfamily23Info( const med_idt       fid,
		 const char*   const meshname,
		 const int           famit,
		 char*         const familyname,
		 med_int*      const attributenumber,
		 med_int*      const attributevalue,
		 char*         const attributedes,
		 med_int*      const familynumber,
		 char*         const groupname);

/* Equivalence    */

MEDC_EXPORT med_err
MEDequivalenceCr(const med_idt      fid,
		 const char * const meshname,
		 const char * const equivname,
		 const char * const description);

MEDC_EXPORT med_err
MEDequivalenceCorrespondenceWr(const med_idt            fid,
			       const char * const       meshname,
			       const char * const       equivname,
			       const med_int            numdt,
			       const med_int            numit,
			       const med_entity_type    entitype,
			       const med_geometry_type  geotype,
			       const med_int            nentity,
			       const med_int * const    correspondence);

MEDC_EXPORT med_err
MEDequivalenceCorrespondenceRd(const med_idt            fid,
			       const char * const       meshname,
			       const char * const       equivname,
			       const med_int            numdt,
			       const med_int            numit,
			       const med_entity_type    entitype,
			       const med_geometry_type  geotype,
			       med_int * const          correspondence);

MEDC_EXPORT med_int
MEDnEquivalence(const med_idt      fid,
		const char * const meshname);

MEDC_EXPORT med_err
MEDequivalenceInfo(const med_idt      fid,
		   const char * const meshname,
		   const int          equivit,
		   char * const       equivname,
		   char * const       equivdescription,
		   med_int * const    nstep,
		   med_int * const    nocstpncorrespondence);

MEDC_EXPORT med_err
MEDequivalenceComputingStepInfo(const med_idt      fid,
				const char * const meshname,
				const char * const equivname,
				const int          csit,
				med_int * const    numdt,
				med_int * const    numit,
				med_int * const    ncorrespondence );


MEDC_EXPORT med_err
MEDequivalenceCorrespondenceSizeInfo(const med_idt              fid,
				     const char * const         meshname,
				     const char * const         equivname,
				     const med_int              numdt,
				     const med_int              numit,
				     const int                  corit,
				     med_entity_type * const    entitype,
				     med_geometry_type* const   geotype,
				     med_int * const            nentity );

/*Uses case equivalencename from Mesh element iteration*/
/* MEDC_EXPORT med_err */
/* MEDequivalenceInfoByName(const med_idt      fid, */
/* 			 const char * const meshname, */
/* 			 const char * const equivname, */
/* 			 char * const       equivdescription, */
/* 			 med_int * const    nstep, */
/* 			 med_int * const    nocstpncorrespondence); */

MEDC_EXPORT med_err
MEDequivalenceCorrespondenceSize(const med_idt             fid,
				 const char * const        meshname,
				 const char * const        equivname,
				 const med_int             numdt,
				 const med_int             numit,
				 const med_entity_type     entitype,
				 const med_geometry_type   geotype,
				 med_int * const           nentity );



/* MEDC_EXPORT med_err */
/* MEDmeshEquivalenceInfo(const med_idt            fid, */
/* 		       const char * const       meshname, */
/* 		       const med_int            numdt, */
/* 		       const med_int            numit, */
/* 		       const med_entity_type    entitype, */
/* 		       const med_geometry_type  geotype, */
/* 		       char * const             equivname ); */

/* Joints */

MEDC_EXPORT med_err
MEDsubdomainJointCr(const med_idt      fid,
		    const char * const localmeshname,
		    const char * const jointname,
		    const char * const description,
		    const med_int      domainnumber,
		    const char * const remotemeshname);

MEDC_EXPORT med_err
MEDsubdomainCorrespondenceWr(const med_idt            fid,
			     const char * const       meshname,
			     const char * const       jointname,
			     const med_int            numdt,
			     const med_int            numit,
			     const med_entity_type    localentitytype,
			     const med_geometry_type  localgeotype,
			     const med_entity_type    remoteentitytype,
			     const med_geometry_type  remotegeotype,
			     const med_int            nentity,
			     const med_int * const    correspondence);

MEDC_EXPORT med_int
MEDnSubdomainJoint(const med_idt      fid,
		   const char * const meshname);

MEDC_EXPORT med_err
MEDsubdomainJointInfo(const med_idt      fid,
		      const char * const meshname,
		      const int          jointit,
		      char * const       jointname,
		      char * const       description,
		      med_int * const    domainnumber,
		      char * const       remotemeshname,
		      med_int * const    nstep,
		      med_int * const    nocstpncorrespondence);

MEDC_EXPORT med_err
MEDsubdomainComputingStepInfo(const med_idt      fid,
			      const char * const meshname,
			      const char * const jointname,
			      const int          csit,
			      med_int * const    numdt,
			      med_int * const    numit,
			      med_int * const    ncorrespondence );

MEDC_EXPORT med_err
MEDsubdomainCorrespondenceSizeInfo(const med_idt              fid,
				   const char * const         meshname,
				   const char * const         jointname,
				   const med_int              numdt,
				   const med_int              numit,
				   const int                  corit,
				   med_entity_type   * const  localentitytype,
				   med_geometry_type * const  localgeotype,
				   med_entity_type   * const  remoteentitytype,
				   med_geometry_type * const  remotegeotype,
				   med_int * const            nentity );

MEDC_EXPORT med_err
MEDsubdomainCorrespondenceSize(const med_idt              fid,
			       const char * const         meshname,
			       const char * const         jointname,
			       const med_int              numdt,
			       const med_int              numit,
			       const med_entity_type      localentitytype,
			       const med_geometry_type    localgeotype,
			       const med_entity_type      remoteentitytype,
			       const med_geometry_type    remotegeotype,
			       med_int * const            nentity );
MEDC_EXPORT med_err
MEDsubdomainCorrespondenceRd(const med_idt            fid,
			     const char * const       meshname,
			     const char * const       jointname,
			     const med_int            numdt,
			     const med_int            numit,
			     const med_entity_type    localentitytype,
			     const med_geometry_type  localgeotype,
			     const med_entity_type    remoteentitytype,
			     const med_geometry_type  remotegeotype,
			     med_int * const          correspondence);

MEDC_EXPORT
med_err MEDmeshGlobalNumberWr(const med_idt               fid,
			      const char*  const          meshname,
			      const med_int               numdt,
			      const med_int               numit,
			      const med_entity_type       entitytype,
			      const med_geometry_type     geotype,
			      const med_int               nentity,
			      const med_int * const       number);
MEDC_EXPORT
med_err MEDmeshGlobalNumberRd(const med_idt               fid,
			      const char*  const          meshname,
			      const med_int               numdt,
			      const med_int               numit,
			      const med_entity_type       entitytype,
			      const med_geometry_type     geotype,
			      med_int * const             number);

/* Routines de niveau intermediaire */
MEDC_EXPORT
med_err MEDmeshNodeWr(const med_idt                  fid,
		      const char            * const  meshname,
		      const med_int                  numdt,
		      const med_int                  numit,
		      const med_float                dt,
		      const med_switch_mode          switchmode,
		      const med_int                  nentity,
		      const med_float       * const  coordinate,
		      const med_bool                 withnodename,
		      const char            * const  nodename,
		      const med_bool                 withnodenumber,
		      const med_int         * const  nodenumber,
		      const med_bool                 withfamnumber,
		      const med_int         * const  famnumber);

MEDC_EXPORT
med_err MEDmeshNodeRd(const med_idt                  fid,
		      const char            * const  meshname,
		      const med_int                  numdt,
		      const med_int                  numit,
		      const med_switch_mode          switchmode,
		      med_float             * const  coordinate,
		      med_bool              * const  withnodename,
		      char                  * const  nodename,
		      med_bool              * const  withnodenumber,
		      med_int               * const  nodenumber,
		      med_bool              * const  withfamnumber,
		      med_int               * const  famnumber);

MEDC_EXPORT
med_err MEDmeshElementWr(const med_idt                  fid,
			 const char            * const  meshname,
			 const med_int                  numdt,
			 const med_int                  numit,
			 const med_float                dt,
			 const med_entity_type          entitype,
			 const med_geometry_type        geotype,
			 const med_connectivity_mode    cmode,
			 const med_switch_mode          switchmode,
			 const med_int                  nentity,
			 const med_int         * const  connectivity,
			 const med_bool                 withelementname,
			 const char            * const  elementname,
			 const med_bool                 withelementnumber,
			 const med_int         * const  elementnumber,
			 const med_bool                 withfamnumber,
			 const med_int         * const  famnumber);
MEDC_EXPORT
med_err MEDmeshElementRd(const med_idt                  fid,
			 const char            * const  meshname,
			 const med_int                  numdt,
			 const med_int                  numit,
			 const med_entity_type          entitype,
			 const med_geometry_type        geotype,
			 const med_connectivity_mode    cmode,
			 const med_switch_mode          switchmode,
			 med_int               * const  connectivity,
			 med_bool              * const  withelementname,
			 char                  * const  elementname,
			 med_bool              * const  withelementnumber,
			 med_int               * const  elementnumber,
			 med_bool              * const  withfamnumber,
			 med_int               * const  famnumber);

/* Profile */
MEDC_EXPORT med_int
MEDnProfile(const med_idt fid );

MEDC_EXPORT med_err
MEDprofileInfo(const med_idt          fid,
	       const int              profileit,
	       char    *     const    profilename,
	       med_int *     const    profilesize );
MEDC_EXPORT med_err
MEDprofileWr(const med_idt        fid,
	     const char* const    profilename,
	     const med_int        profilesize,
	     const med_int* const profilearray);

MEDC_EXPORT med_int
MEDprofileSizeByName(const med_idt fid, const char * const profilename);

MEDC_EXPORT med_err
MEDprofileRd(const med_idt      fid,
	     const char * const profilename,
	     med_int * const    profilearray );

/* Filter */

MEDC_EXPORT med_err
MEDfilterEntityCr(const med_idt fid,
		  const med_int nentity,
		  const med_int nvaluesperentity,
		  const med_int nconstituentpervalue,
		  const med_int constituentselect,
		  const med_switch_mode switchmode,
		  const med_storage_mode storagemode,
		  const char * const profilename,
		  const med_int filterarraysize,
		  const med_int *filterarray, med_filter* const filter);

MEDC_EXPORT med_err
MEDfilterBlockOfEntityCr(const med_idt          fid,
			 const med_int          nentity,
			 const med_int          nvaluesperentity,
			 const med_int          nconstituentpervalue,
			 const med_int          constituentselect,
			 const med_switch_mode  switchmode,
			 const med_storage_mode storagemode,
			 const char * const     profilename,
			 const med_size         start,
			 const med_size         stride,
			 const med_size         count,
			 const med_size         blocksize,
			 const med_size         lastblocksize,
			 med_filter*    const   filter);

MEDC_EXPORT med_err
MEDfilterDeAllocate(const int nfilter,
		    med_filter * filter);

MEDC_EXPORT med_filter*
MEDfilterAllocate(const int nfilter);

MEDC_EXPORT med_err
MEDfilterClose(  med_filter * const filter);

/* Localization */
MEDC_EXPORT med_err
MEDlocalizationWr(const med_idt fid,
		  const char * const localizationname,
		  const med_geometry_type geotype,
		  const med_int spacedimension,
		  const med_float* const elementcoordinate,
		  const med_switch_mode switchmode,
		  const med_int nipoint,
		  const med_float* const ipointcoordinate,
		  const med_float* const weight,
		  const char *     const  geointerpname,
		  const char *     const  ipointstructmeshname);


MEDC_EXPORT med_err
MEDlocalizationRd(const med_idt                 fid,
		  const char*            const  localizationname,
		  const med_switch_mode         switchmode,
		  med_float*             const  elementcoordinate,
		  med_float*             const  ipointcoordinate,
		  med_float*             const  weight);

MEDC_EXPORT med_int
MEDnLocalization(const med_idt fid );

MEDC_EXPORT med_err
MEDlocalizationInfo(const med_idt             fid,
		    const int                 localizationit,
		    char              * const localizationname,
		    med_geometry_type * const geotype,
		    med_int           * const spacedimension,
		    med_int           * const nipoint,
		    char *              const geointerpname,
		    char *              const sectionmeshname,
		    med_int           * const nsectionmeshcell,
		    med_geometry_type * const sectiongeotype);

MEDC_EXPORT med_err
MEDlocalizationInfoByName(const med_idt             fid,
			  const char        * const localizationname,
			  med_geometry_type * const geotype,
			  med_int           * const spacedimension,
			  med_int           * const nipoint,
			  char *              const geointerpname,
			  char *              const sectionmeshname,
			  med_int           * const nsectionmeshcell,
			  med_geometry_type * const sectiongeotype);


/* Link */
MEDC_EXPORT med_err
MEDlinkWr(const med_idt        fid,
	  const char   * const meshname,
	  const char   * const link);

MEDC_EXPORT med_err
MEDlinkRd(const med_idt     fid,
	  const char* const meshname,
	  char* const       link);

MEDC_EXPORT med_int
MEDnLink(const med_idt fid );
MEDC_EXPORT med_int
MEDlinkInfoByName(const med_idt             fid,
		  const char        * const meshname );

MEDC_EXPORT med_err
MEDlinkInfo(const med_idt             fid,
	    const int                 linkit,
	    char              * const meshname,
	    med_int           * const linksize );

/* Variables scalaires */

/* Struct Elements */

MEDC_EXPORT med_geometry_type
MEDstructElementCr(const med_idt                 fid,
		   const char*             const modelname,
		   const med_int                 modeldim,
		   const char*             const supportmeshname,
		   const med_entity_type         sentitytype,
		   const med_geometry_type       sgeotype
		   );

MEDC_EXPORT med_int
MEDnStructElement(const med_idt      fid);

MEDC_EXPORT med_err
MEDstructElementConstAttWithProfileWr(const med_idt                  fid,
				      const char*              const modelname,
				      const char*              const constattname,
				      const med_attribute_type       constatttype,
				      const med_int                  ncomponent,
				      const med_entity_type          sentitytype,
				      const char*              const profilename,
				      const void*              const value
				      );

MEDC_EXPORT med_err
MEDstructElementConstAttWr(const med_idt                  fid,
			   const char*              const modelname,
			   const char*              const constattname,
			   const med_attribute_type       constatttype,
			   const med_int                  ncomponent,
			   const med_entity_type          sentitytype,
			   const void*              const value
			   );

MEDC_EXPORT med_err
MEDstructElementInfoByName(const med_idt             fid,
			   const char *        const modelname,
			   med_geometry_type * const mgeotype,
			   med_int*            const modeldim,
			   char*               const supportmeshname,
			   med_entity_type*    const sentitytype,
			   med_int*            const snnode,
			   med_int*            const sncell,
			   med_geometry_type*  const sgeotype,
			   med_int*            const nconstantatribute,
			   med_bool*           const anyprofile,
			   med_int*            const nvariableattribute
			   );

MEDC_EXPORT med_err
MEDstructElementInfo(const med_idt             fid,
		     const int                 mit,
		     char *              const modelname,
		     med_geometry_type * const mgeotype,
		     med_int*            const modeldim,
		     char*               const supportmeshname,
		     med_entity_type*    const sentitytype,
		     med_int*            const snnode,
		     med_int*            const sncell,
		     med_geometry_type*  const sgeotype,
		     med_int*            const nconstantattribute,
		     med_bool*           const anyprofile,
		     med_int*            const nvariableattribute
		     );

MEDC_EXPORT med_err
MEDstructElementConstAttInfoByName(const med_idt             fid,
				   const char*         const modelname,
				   const char*         const constattname,
				   med_attribute_type* const constatttype,
				   med_int*            const ncomponent,
				   med_entity_type*    const sentitytype,
				   char*               const profilename,
				   med_int*            const profilesize
				   );
MEDC_EXPORT med_err
MEDstructElementConstAttInfo(const med_idt             fid,
			     const char*         const modelname,
			     const int                 attit,
			     char*               const constattname,
			     med_attribute_type* const constatttype,
			     med_int*            const ncomponent,
			     med_entity_type*    const sentitytype,
			     char*               const profilename,
			     med_int*            const profilesize
			     );

MEDC_EXPORT med_err
MEDstructElementConstAttRd(const med_idt                  fid,
			   const char*              const modelname,
			   const char*              const constattname,
			   void*                    const value
			   );

MEDC_EXPORT int
MEDstructElementAttSizeof( med_attribute_type atttype );

MEDC_EXPORT med_err
MEDstructElementVarAttCr(const med_idt                  fid,
			 const char*              const modelname,
			 const char*              const varattname,
			 const med_attribute_type       varatttype,
			 const med_int                  ncomponent
			 );

MEDC_EXPORT med_err
MEDstructElementVarAttInfoByName(const med_idt                   fid,
				 const char*               const modelname,
				 const char*               const varattname,
				       med_attribute_type* const varatttype,
				       med_int*            const ncomponent
				 );

MEDC_EXPORT med_err
MEDstructElementVarAttInfo(const med_idt                   fid,
			   const char*               const modelname,
			   const int                       attit,
			         char*               const varattname,
			         med_attribute_type* const varatttype,
			         med_int*            const ncomponent
			   );

MEDC_EXPORT med_err
MEDmeshStructElementVarAttWr(const med_idt                  fid,
			     const char*              const meshname,
			     const med_int                  numdt,
			     const med_int                  numit,
			     const med_geometry_type        mgeotype,
			     const char*              const varattname,
			     const med_int                  nentity,
			     const void*              const value
			     );
MEDC_EXPORT med_err
MEDmeshStructElementVarAttRd(const med_idt                  fid,
			     const char*              const meshname,
			     const med_int                  numdt,
			     const med_int                  numit,
			     const med_geometry_type        mgeotype,
			     const char*              const varattname,
			     void*                    const value
			     );

MEDC_EXPORT med_err
MEDstructElementName(const med_idt                 fid,
		     const med_geometry_type       mgeotype,
		     char *                  const modelname);


MEDC_EXPORT med_geometry_type
MEDstructElementGeotype(const med_idt                 fid,
			const char *            const modelname);

/* Maillages support aux mod�les d'�l�ments de structure */

MEDC_EXPORT med_err
MEDsupportMeshCr(const med_idt       fid,
		 const char* const   supportmeshname,
		 const med_int       spacedim,
		 const med_int       meshdim,
		 const char* const   description,
		 const med_axis_type axistype,
		 const char* const   axisname,
		 const char* const   axisunit
		 );



MEDC_EXPORT med_err
MEDsupportMeshInfoByName(const med_idt         fid,
			 const char *    const supportmeshname,
			 med_int *       const spacedim,
			 med_int *       const meshdim,
			 char *          const description,
			 med_axis_type * const axistype,
			 char *          const axisname,
			 char *          const axisunit);

MEDC_EXPORT med_err
MEDsupportMeshInfo(const med_idt            fid,
		   const int                meshit,
		   char   *           const supportmeshname,
		   med_int *          const spacedim,
		   med_int *          const meshdim,
		   char *             const description,
		   med_axis_type *    const axistype,
		   char *             const axisname,
		   char *             const axisunit);

MEDC_EXPORT med_int
MEDnSupportMesh(const med_idt fid);

MEDC_EXPORT med_int
MEDsupportMeshnAxis(const med_idt fid, const int meshit);

MEDC_EXPORT med_int
MEDsupportMeshnAxisByName(const med_idt fid, const char * const meshname);


MEDC_EXPORT med_idt
_MEDmeshDatagroupOpen(const med_idt               fid,
		      const char*  const          meshname,
		      char *       const          usedpath,
		      med_bool *   const          isasupportmesh
		     );


  /* Parameter */
MEDC_EXPORT med_err
MEDparameterCr(const med_idt fid,
	       const char * const paramname,
	       const med_parameter_type paramtype,
	       const char* const description,
	       const char * const dtunit
	       );

MEDC_EXPORT med_int
MEDnParameter(const med_idt fid);

MEDC_EXPORT med_err
MEDparameterInfo(const med_idt            fid,
		 const int                paramit,
		 char   *             const paramname,
		 med_parameter_type * const paramtype,
		 char *               const description,
		 char *               const dtunit,
		 med_int *            const nstep);

MEDC_EXPORT med_err
MEDparameterInfoByName(const med_idt        fid,
		       const char   *       const paramname,
		       med_parameter_type * const paramtype,
		       char *               const description,
		       char *               const dtunit,
		       med_int *            const nstep);

MEDC_EXPORT med_err
MEDparameterValueWr(const med_idt              fid,
		    const char*  const         paramname,
		    const med_int              numdt,
		    const med_int              numit,
		    const med_float            dt,
		    const unsigned char* const value);

MEDC_EXPORT med_err
MEDparameterValueRd(const med_idt              fid,
		    const char*  const         paramname,
		    const med_int              numdt,
		    const med_int              numit,
		    unsigned char* const value);


MEDC_EXPORT med_err
MEDparameterComputationStepInfo(const med_idt      fid,
				const char * const paramname,
				const int          csit,
				med_int * const    numdt,
				med_int * const    numit,
				med_float * const  dt );

  /* Fonctions d'interpolation */
MEDC_EXPORT med_err
MEDinterpCr(const med_idt                 fid,
	    const char*             const interpname,
	    const med_geometry_type       geotype,
	    const med_bool                cellnodes,
	    const med_int                 nvariable,
	    const med_int                 maxdegree,
	    const med_int                 nmaxcoef
	    );

MEDC_EXPORT med_err
MEDinterpBaseFunctionWr( const med_idt          fid,
			 const char*      const interpname,
			 const med_int          basisfuncit,
			 const med_int          ncoef,
			 const med_int*   const power,
			 const med_float* const coefficient);

MEDC_EXPORT med_int
MEDnInterp(const med_idt fid);

MEDC_EXPORT med_err
MEDinterpInfo(const med_idt                 fid,
	      const int                      interpit,
	            char*              const interpname,
	            med_geometry_type* const geotype,
	            med_bool*          const cellnode,
	            med_int*           const nbasisfunc,
	            med_int*           const nvariable,
	            med_int*           const maxdegree,
	            med_int*           const nmaxcoef
	      );

MEDC_EXPORT med_err
MEDinterpInfoByName(const med_idt                   fid,
		    const char*               const interpname,
		           med_geometry_type*       geotype,
		           med_bool*          const cellnode,
		           med_int*           const nbasisfunc,
		           med_int*           const nvariable,
		           med_int*           const maxdegree,
		           med_int*           const nmaxcoef
		    );

MEDC_EXPORT med_err
MEDinterpBaseFunctionRd( const med_idt          fid,
			 const char*      const interpname,
			 const int              basisfuncit,
			       med_int*         ncoef,
			       med_int*   const power,
			 med_float* const coefficient);

MEDC_EXPORT med_int
MEDinterpBaseFunctionCoefSize( const med_idt          fid,
			       const char*      const interpname,
			       const med_int          basisfuncit);
#ifdef __cplusplus
}
#endif

#endif /* MED_PROTO_H */




