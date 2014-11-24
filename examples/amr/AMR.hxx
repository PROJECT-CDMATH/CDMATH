/*
 * AMR.hxx
 *
 *  Created on: 2 juil. 2014
 *      Author: mekkas
 */

#ifndef AMR_HXX_
#define AMR_HXX_

namespace ParaMEDMEM
{
    class MEDCouplingFieldDouble;
    class MEDCouplingIMesh;
    class MEDCouplingCartesianAMRMesh;
    class MEDCouplingAMRAttribute;
    class DataArrayDouble;
}

#include <MEDCouplingAutoRefCountObjectPtr.hxx>

namespace INTERP_KERNEL
{
    class BoxSplittingOptions;
}

#include "IterativeProblem.hxx"
#include <string>
#include <vector>


class AMR {

public:
    AMR(int maxLevels,
        int frequencyOfRefinement,
        std::vector<double> efficiencyGoal,
        std::vector<double> efficiencyThreshold,
        std::vector<int> maximumNbOfCellsInPatch,
        std::vector<int> minimumPatchLength,
        std::vector<int> maximumPatchLength,
        std::vector< std::vector<int> > coeficientsRefinement) ;

    ~AMR();

    int getMaximumLevels( void ) ;

    ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingAMRAttribute> getFields( void ) const ;

    int getFrequencyOfRefinement( void ) const ;

    void setFrequencyOfRefinement( int frequencyOfRefinement ) ;

    double getEfficiencyGoalAtLevel( int iLevel ) const ;

    std::vector<double> getEfficiencyGoal( void ) const ;

    double getEfficiencyThresholdAtLevel( int iLevel ) const ;

    std::vector<double> getEfficiencyThreshold( void ) const ;

    int getMaximumNbOfCellsInPatchAtLevel( int iLevel ) const ;

    IterativeProblem* getIterativeProblem( void ) const ;

    std::vector<int> getMaximumNbOfCellsInPatch( void ) const ;

    int getMinimumPatchLengthAtLevel( int iLevel ) const ;

    std::vector<int> getMinimumPatchLength( void ) const ;

    int getMaximumPatchLengthAtLevel( int iLevel ) const ;

    std::vector<int> getMaximumPatchLength( void ) const ;

    std::vector<int> getCoeficientsRefinementAtLevel( int iLevel ) const ;

    std::vector< std::vector<int> > getCoeficientsRefinement( void ) const ;

    std::vector< std::pair<std::string,int> > getFieldsInfo( void ) const ;

    void PostTreatment(int it, double time, std::string nameOfField, std::string fileName, bool fromscratch) const;

    void compute(const IterativeProblem& IterativeProblem) ;

    void setFieldsInfo(std::vector< std::pair<std::string,int> > fieldsInfos) ;

    static ParaMEDMEM::MEDCouplingFieldDouble*
    buildFieldWithGhostFromFieldWithoutGhost(int numberOfCellsGhost,
                                             const ParaMEDMEM::MEDCouplingIMesh* mesh,
                                             const ParaMEDMEM::MEDCouplingFieldDouble* constArrWithoutGhost);

    static ParaMEDMEM::MEDCouplingFieldDouble*
    buildFieldWithoutGhostFromFieldWithGhost(int numberOfCellsGhost,
                                             const ParaMEDMEM::MEDCouplingIMesh* meshWithoutGhost,
                                             const ParaMEDMEM::MEDCouplingFieldDouble* fieldWithGhost);

    double
    unsteadyAMRDriver(double currentTime, const IterativeProblem& IterativeProblem);

    void
    unsteadyAMRDriverHigherLevels(int curLevel, double currentTime, const IterativeProblem& IterativeProblem);

    void initialize(const ParaMEDMEM::MEDCouplingIMesh* coarseMesh,
                    std::vector< std::pair<std::string,int> > fieldsInfos,
                    const IterativeProblem& IterativeProblem);

    std::string writeVTKAMRFieldOnRecurse(int it, std::string nameOfField, std::string fileName) const;

    std::string writeVTKAMRFieldPatches(int it, std::string nameOfField, std::string fileName) const;

    void writePVD(std::string fileName, std::string filevt, double time, bool fromScratch) const ;

    void refinement(const std::vector<const INTERP_KERNEL::BoxSplittingOptions*>& bsos,const IterativeProblem& IterativeProblem) ;

    double computeNumberOfCoarseCellsAtHigherLevel(const ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingCartesianAMRMesh> amr, int level) const ;

protected:

    int _maxLevels;
    int _frequencyOfRefinement;
    std::vector<double> _efficiencyGoal;
    std::vector<double> _efficiencyThreshold;
    std::vector<int> _maximumNbOfCellsInPatch;
    std::vector<int> _maximumPatchLength;
    std::vector<int> _minimumPatchLength;
    std::vector< std::vector<int> > _coeficientsRefinement;
    ParaMEDMEM::MEDCouplingAutoRefCountObjectPtr<ParaMEDMEM::MEDCouplingAMRAttribute> _fields;
    std::vector< std::pair<std::string,int> > _fieldsInfos;
//  IterativeProblem* _IterativeProblem;
};

#endif /* AMR_HXX_ */
