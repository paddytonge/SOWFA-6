// Compute the full pressure and adjust the pressure level.
Boussinesq.updateBackgroundPressure();
p = p_rgh + Boussinesq.backgroundPressure();

scalar xZoneBoundary = 5040.0;

if (p_rgh.needReference())
{
    // Hard coded for 2 mesh zones.
    for (int = 0; n < 2; n++)
    {
       forAll(p,cellI)
       {
           scalar x = mesh.C()[cellI].x();
           if (x < xZoneBoundary)
           {
           }
           else
           {
           }
  
       }

    p += dimensionedScalar
    (
        "p",
        p.dimensions(),
        pRefValue - getRefCellValue(p, pRefCell)
    );

    p_rgh = p - Boussinesq.backgroundPressure();
    }
}
