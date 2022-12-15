// Compute the full pressure and adjust the pressure level.
Boussinesq.updateBackgroundPressure();
p = p_rgh + Boussinesq.backgroundPressure();



if (p_rgh.needReference())
{

    List<scalar> refCellValues(2,0.0);
    for (int i = 0; i < 2; i++)
    {
        refCellValues[i] = (refCells[i] >= 0 ? p[refCells[i]] : 0.0);
        returnReduce(refCellValues[i], sumOp<scalar>());
    }
    Info << "refCellValues: " << endl;
    Info << refCellValues << endl;

    // Hard coded for 2 mesh zones.
    forAll(p,cellI)
    {
        scalar x = mesh.C()[cellI].x();
        if (x < xDiv)
        {
            p[cellI] += pRefValue - refCellValues[0];
/*
            p[cellI] += dimensionedScalar
            (
                 "p",
                 p.dimensions(),
                 pRefValue - refCellValues[0]
            );
*/
        }
        else
        {
            p[cellI] += pRefValue - refCellValues[1];
/*
            p[cellI] += dimensionedScalar
            (
                 "p",
                 p.dimensions(),
                 pRefValue - refCellValues[1]
            );
*/
        }
    }

/*
    p += dimensionedScalar
    (
        "p",
        p.dimensions(),
        pRefValue - getRefCellValue(p, pRefCell)
    );
*/

    p_rgh = p - Boussinesq.backgroundPressure();
}
