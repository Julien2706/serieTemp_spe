cp %1/cblas.* .
cp %1/gsl.* .
rm %1/*.*
mv cblas.* %1
mv gsl.* %1