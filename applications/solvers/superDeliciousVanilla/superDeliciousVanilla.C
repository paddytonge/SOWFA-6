/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    superDeliciousVanilla (the code name for the new unified solver)

Description
    Transient solver for wind-energy and atmospheric boundary layer flows.
    The incompressible equations are solved along with a Boussinesq buoyancy
    term. There is flexibility in turbulence modeling. Although the solver
    is primarily meant for and tested for large-eddy simulations, it could
    also be used with a RANS turbulence model.

    The code includes planetary Coriolis forces, pressure gradient forces
    driving the wind, ability to use mesoscale forcings, coupling with
    atmospheric-style lower rough-wall boundary conditions, and coupling
    with actuator turbine models.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "wallDist.H"
#include "fixedFluxPressureFvPatchScalarField.H"
#include "timeVaryingMappedInletOutletFvPatchField.H"
#include "fvOptions.H"
#include "pimpleControl.H"
#include "ABL.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "postProcess.H"

    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createControl.H"
    #include "createFields.H"
    #include "createTimeControls.H"
    #include "CourantNo.H"
    #include "setInitialDeltaT.H"
    #include "computeDivergence.H"
    #include "createDivSchemeBlendingField.H"

    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info << endl << "Starting time loop" << endl;

    // Update boundary conditions before starting in case anything needs
    // updating, for example after using mapFields to interpolate initial
    // field.
    Info << "U " << U << endl;
    Info << "phi " << phi << endl;
  //U.correctBoundaryConditions();
  //Info << "U " << U << endl;
  //Info << "phi " << phi << endl;
  //phi = linearInterpolate(U) & mesh.Sf();
  //Info << "phi " << phi << endl;
    #include "turbulenceCorrect.H"
    /*
    for (int i = 0; i < Pstream::nProcs(); i++)
    {
        if (Pstream::myProcNo() == 1)
        {
            sleep(2);
        }
        if (Pstream::myProcNo() == i)
        {
            Pout << "Processor No. " << i << endl;
            Pout << "nOldTimes " << T.nOldTimes() << endl;
            Pout << "T: " << endl << T << endl;
            Pout << "T.oldTime: " << endl << T.oldTime() << endl;
            Pout << "rhok: " << endl << Boussinesq.rhok() << endl;
            Pout << "buoyancyTerm: " << endl << Boussinesq.buoyancyTerm() << endl;
        }
    }
    */
    //T.correctBoundaryConditions();
    Info << "T.nOldTimes() " << T.nOldTimes() << endl;
    Info << "U.nOldTimes() " << U.nOldTimes() << endl;
    Info << "T: " << T << endl;
    Info << "T.oldTime: " << T.oldTime() << endl;
  //Boussinesq.updateDensityField();
    for (int i = 0; i < Pstream::nProcs(); i++)
    {
        if (Pstream::myProcNo() == 1)
        {
            sleep(2);
        }
        if (Pstream::myProcNo() == i)
        {
            Pout << "rhok: " << Boussinesq.rhok() << endl;
            Pout << "buoyancyTerm: " << Boussinesq.buoyancyTerm() << endl;
        }
    }

    // Time stepping loop.
    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"
        #include "updateDivSchemeBlendingField.H"

        word timeNameOld = runTime.timeName();

        runTime++;

        Info << "Time Step = " << runTime.timeIndex() << ", ";
        Info << "Time = " << timeNameOld << " to " << runTime.timeName() << " s";
        Info << nl << endl;

        // Test to see if simulation begins without t^(n-1) (*_0) fields.
        // Without those fields, the backward d/dt scheme reverts to Euler
        // implicit in outer iteration 0, but uses full backward in
        // iterations >0, and the U,T^(n-1) data is incorrect.  Outer
        // iterations >0 are only required to achieve 2nd-order accuracy
        // in time, but since that is not possible in the first time step
        // without t^(n-1) data, we really only need 1 outer iteration,
        // anyway.
        bool limitOuterLoop = ((U.nOldTimes() == 0) || (T.nOldTimes() == 0)) ? true : false;


        // Outer-iteration loop.
        while (pimple.loop())
        {
            // Update the source terms.
            momentumSourceTerm.update(pimple.finalPimpleIter());
            temperatureSourceTerm.update(pimple.finalPimpleIter());

            // Update damping layers forcing
            spongeLayers.update();
    
            Boussinesq.update();

            // Predictor step.
            Info << "   Predictor" << endl;

            Info << "T: " << T << endl;
            Info << "rhok: " << Boussinesq.rhok() << endl;
            Info << "buoyancyTerm: " << Boussinesq.buoyancyTerm() << endl;
            #include "UEqn.H"
            #include "turbulenceCorrect.H"
            #include "TEqn.H"
            Info << "T: " << T << endl;
            Info << "rhok: " << Boussinesq.rhok() << endl;
            Info << "buoyancyTerm: " << Boussinesq.buoyancyTerm() << endl;
            Info << "T.nOldTimes() " << T.nOldTimes() << endl;
            Info << "U.nOldTimes() " << U.nOldTimes() << endl;

            // Corrector steps.
            int corrIter = 1;
            while (pimple.correct())
            {
                Info << "   Corrector Step " << corrIter << endl;

                #include "pEqn.H"
                #include "turbulenceCorrect.H"
                #include "TEqn.H"
                Info << "T.nOldTimes() " << T.nOldTimes() << endl;

                corrIter++;
            }

            // If starting without the t^(n-1) data, advance through the remaining
            // outer iterations without doing anything by calling pimple.loop().
            if (limitOuterLoop)
            {
                while (pimple.loop());
                Info << "T.nOldTimes() " << T.nOldTimes() << endl;
                Info << "U.nOldTimes() " << U.nOldTimes() << endl;
                Info << "p_rgh.nOldTimes() " << p_rgh.nOldTimes() << endl;
                Info << endl;
                break;
            }

            Info << endl;
        }

        // Compute the continuity errors.
        #include "computeDivergence.H"

        // Update timeVaryingMappedInletOutlet parameters
        #include "updateFixesValue.H"

        // Write the solution if at write time.
        runTime.write();

        Info << "T: " << T << endl;
        Info << "T.oldTime(): " << T.oldTime() << endl;
        Info << "T.oldTime().oldTime(): " << T.oldTime().oldTime() << endl;
        Info << "rhok: " << Boussinesq.rhok() << endl;
        Info << "buoyancyTerm: " << Boussinesq.buoyancyTerm() << endl;
        Info << "T.nOldTimes() " << T.nOldTimes() << endl;
        Info << "U.nOldTimes() " << U.nOldTimes() << endl;
        Info << "p_rgh.nOldTimes() " << p_rgh.nOldTimes() << endl;

        // Report timing.
        Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
             << "  ClockTime = " << runTime.elapsedClockTime() << " s"
             << nl << endl;
    }

    Info << "Ending the simulation" << endl;

    return 0;
}


// ************************************************************************* //
